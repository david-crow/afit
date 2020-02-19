# imports
library(rEDM)
library(plyr)
library(ggplot2)

# for saving
frame_dir <- "/Users/david/Documents/masters/thesis/analysis/r/avas/frames/"
image_dir <- "/Users/david/Documents/masters/thesis/analysis/r/avas/images/"
# image_dir <- "/Users/david/Desktop/"

# load data
df <- read.csv("/Users/david/Documents/masters/thesis/data/avas/edm/avas8.csv")

# segment into library/prediction (train/test, if you will)
n <- NROW(df)
lib <- c(1, floor(2/3 * n))
pred <- c(floor(2/3 * n) + 1, n)

# segregate the columns
altitude <- df[c("Time", "Altitude")]
airspeed <- df[c("Time", "Airspeed")]
pitch <- df[c("Time", "Pitch")]

# radians -> degrees
pitch$Pitch <- pitch$Pitch * 180 / pi

# scale
altitude$Altitude <- scale(altitude$Altitude)
airspeed$Airspeed <- scale(airspeed$Airspeed)
pitch$Pitch <- scale(pitch$Pitch)

# ---------------------------------------------------------------------
# functions
# ---------------------------------------------------------------------

# identify the optimal embedding dimension (E)
compute_e <- function(var) {
    simplex_output <- simplex(var, lib=lib, pred=pred, E=1:10)
    simplex_output[is.na(simplex_output)] <- 0
    simplex_output$rho[simplex_output$rho < 0] <- 0
    
    # write the dataframe
    filename <- paste("e-", gsub("[.]", "-", tolower(colnames(var)[-1])), sep="")
    if (length(colnames(var)) > 2) {
        filename <- paste("e-", gsub("[.]", "-", tolower(colnames(var)[2])), "-and-", gsub("[.]", "-", tolower(colnames(var)[3])), sep="")
    }
    write.csv(simplex_output, paste(frame_dir, filename, ".csv", sep=""), row.names=FALSE)

    E <- which.max(as.numeric(unlist(simplex_output[c("rho")])))
    E <- as.numeric(unlist(simplex_output[c("E")]))[E]

    pdf(paste(image_dir, filename, ".pdf", sep=""))
    plot(rho ~ E, data=simplex_output, type="l", main=colnames(var)[-1], xlab="Embedding Dimension (E)", ylab="Forecast Skill (rho)")
    dev.off()

    return(E)
}

# identify the optimal time to prediction (tp)
compute_tp <- function(var, sequence=seq(0,100,10)) {
    opt_e <- compute_e(var)
    simplex_output <- simplex(var, lib=lib, pred=pred, E=opt_e, tp=sequence)
    tp <- which.max(as.numeric(unlist(simplex_output[c("rho")])))
    tp <- as.numeric(unlist(simplex_output[c("tp")]))[tp]

    # write the dataframe
    filename <- paste("tp-", gsub("[.]", "-", tolower(colnames(var)[-1])), sep="")
    write.csv(simplex_output, paste(frame_dir, filename, ".csv", sep=""), row.names=FALSE)
    
    pdf(paste(image_dir, filename, ".pdf", sep=""))
    plot(rho ~ tp, data=simplex_output, type="l", main=colnames(var)[-1], xaxt="n", xlab="Time to Prediction (tp)", ylab="Forecast Skill (rho)")
    axis(1, at=sequence, labels=0.1 * sequence)
    dev.off()
    
    return(tp)
}

# identify nonlinearity
plot_nonlinearity <- function(var) {
    opt_e <- compute_e(var)
    smap_output <- s_map(var, lib, pred, E=opt_e)

    # write the dataframe
    filename <- paste("smap-", gsub("[.]", "-", tolower(colnames(var)[-1])), sep="")
    write.csv(smap_output, paste(frame_dir, filename, ".csv", sep=""), row.names=FALSE)
    
    pdf(paste(image_dir, filename, ".pdf", sep=""))
    plot(rho ~ theta, data=smap_output, type="l", main=colnames(var)[-1], xlab="Nonlinearity Tuning Parameter (theta)", ylab="Forecast Skill (rho)")
    dev.off()
}

# make and plot predictions
plot_predictions <- function(var, sequence=seq(0,100,10)) {
    opt_e <- compute_e(var)
    opt_tp <- compute_tp(var)
    simplex_output <- simplex(var, lib=lib, pred=pred, E=opt_e, tp=sequence, stats_only=FALSE)
    predictions <- na.omit(simplex_output$model_output[[opt_tp + 1]])

    # write the dataframe
    filename <- paste("predictions-", gsub("[.]", "-", tolower(colnames(var)[-1])), sep="")
    write.csv(predictions, paste(frame_dir, filename, ".csv", sep=""), row.names=FALSE)

    pdf(paste(image_dir, filename, ".pdf", sep=""))
    plot(var, type="l", main=colnames(var)[-1], xlab="Time", ylab="Value")
    lines(pred ~ time, data=predictions, col="blue", lty=2)
    polygon(c(predictions$time, rev(predictions$time)), c(predictions$pred - sqrt(predictions$pred_var),
            rev(predictions$pred + sqrt(predictions$pred_var))), col=rgb(0,0,1,0.3), border=NA)
    dev.off()
}

# conduct and plot analysis of causality
plot_causality <- function(vars=list(altitude, airspeed, pitch), sequence=seq(-100,100,10)) {
    # identify the optimal embedding dimension (E) for each column
    i <- 1
    opt_e <- list()
    var_names <- list()

    for (var in vars) {
        var_names[i] <- colnames(var)[-1]
        opt_e[i] <- compute_e(var)
        i <- i + 1
    }

    opt_e <- unlist(opt_e)
    var_names <- unlist(var_names)

    # get every combination of var1 xmap var2, and add an (E) column that corresponds to the lib column
    params <- expand.grid(lib_column=var_names, target_column=var_names, tp=sequence, stringsAsFactors=FALSE)
    params <- params[params$lib_column != params$target_column,]
    rownames(params) <- NULL
    params$E <- as.integer(mapvalues(params$lib_column, var_names, opt_e, warn_missing=FALSE))

    # compute causality
    output <- do.call(rbind, lapply(seq_len(NROW(params)), function(i) {
        ccm(df, E=params$E[i], random_libs=FALSE, lib_sizes=n, lib_column=params$lib_column[i],
            target_column=params$target_column[i], tp=params$tp[i], silent=TRUE)
    }))

    # add a new column
    output$direction <- paste(output$lib_column, "xmap", output$target_column)

    # write the dataframe
    write.csv(output, paste(frame_dir, "causality.csv", sep=""), row.names=FALSE)

    # plot and save the causalities
    labels <- paste(as.character(round(0.1 * sequence, 2)), "(s)")
    ggplot(output, aes(sequence, rho, colour=direction)) + geom_line() + geom_point() + geom_vline(xintercept=0, linetype="dashed") +
           scale_x_discrete(limits=sequence, labels=labels) + labs(x="Time to Prediction (tp)", y="Forecast Skill (rho)")
    ggsave(paste(image_dir, "causality.pdf", sep=""), device="pdf", width=12, height=8)
}

# conduct and plot analysis of causality means
plot_causality_means <- function(var1, var2) {
    # compute means
    var1_label <- colnames(var1)[-1]
    var2_label <- colnames(var2)[-1]
    opt_e <- compute_e(df[c("Time", var1_label, var2_label)])
    var1_xmap_var2_means <- ccm_means(ccm(df, E=opt_e, num_samples=100, lib_column=var1_label,
                                          target_column=var2_label, lib_sizes=seq(50, 950, 50), random_libs=TRUE, replace=TRUE))
    var2_xmap_var1_means <- ccm_means(ccm(df, E=opt_e, num_samples=100, lib_column=var2_label,
                                          target_column=var1_label, lib_sizes=seq(50, 950, 50), random_libs=TRUE, replace=TRUE))
    
    # write the dataframes
    filename1 <- paste("xmap-", gsub("[.]", "-", tolower(colnames(var1)[-1])), "-and-", gsub("[.]", "-", tolower(colnames(var2)[-1])), sep="")
    write.csv(var1_xmap_var2_means, paste(frame_dir, filename1, ".csv", sep=""), row.names=FALSE)

    filename2 <- paste("xmap-", gsub("[.]", "-", tolower(colnames(var2)[-1])), "-and-", gsub("[.]", "-", tolower(colnames(var1)[-1])), sep="")
    write.csv(var2_xmap_var1_means, paste(frame_dir, filename2, ".csv", sep=""), row.names=FALSE)

    # compute parallel maxima
    y1 <- pmax(0, var1_xmap_var2_means$rho)
    y2 <- pmax(0, var2_xmap_var1_means$rho)

    # plot the means
    title <- paste(var1_label, "&", var2_label)
    limits <- c(min(min(y1), min(y2)), max(max(y1, max(y2))))
    pdf(paste(image_dir, filename1, ".pdf", sep=""))
    plot(var1_xmap_var2_means$lib_size, y1, type="l", col="red", ylim=limits, main=title, xlab="Library Size", ylab="Cross Map Skill (rho)")
    lines(var2_xmap_var1_means$lib_size, y2, col="blue")
    legend(x="topleft", col=c("red", "blue"), lwd=1, bty="n", inset=0.02, cex=0.8,
           legend=c(paste(var1_label, "xmap", var2_label), paste(var2_label, "xmap", var1_label)))
    dev.off()
}

# calls all other functions
run_edm <- function(vars=list(altitude, airspeed, pitch)){
    for (var in vars) {
        plot_nonlinearity(var)
        plot_predictions(var)
    }

    for (var1 in vars) {
        for (var2 in vars) {
            if (colnames(var1)[-1] != colnames(var2)[-1]) {
                plot_causality_means(var1, var2)
            }
        }
    }

    plot_causality(vars)
}
