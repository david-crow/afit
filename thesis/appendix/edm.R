# imports
library(rEDM)
library(plyr)
library(ggplot2)

# load data
df <- read.csv("data/steering.csv")

# segment into library/prediction sets
n <- NROW(df)
lib <- c(1, floor(2/3 * n))
pred <- c(floor(2/3 * n) + 1, n)

# segregate the columns
steering <- df[c("Time", "Steering.wheel.angle")]
left <- df[c("Time", "Left.wheel.RPM")]
right <- df[c("Time", "Right.wheel.RPM")]

# scale the columns
steering$Steering.wheel.angle <- scale(steering$Steering.wheel.angle)
left$Left.wheel.RPM <- scale(left$Left.wheel.RPM)
right$Right.wheel.RPM <- scale(right$Right.wheel.RPM)

# identify the optimal embedding dimension (E)
ComputeE <- function(var) {
    out <- simplex(var, lib=lib, pred=pred, E=1:10)
    out[is.na(out)] <- 0
    out$rho[out$rho < 0] <- 0
    
    E <- which.max(as.numeric(unlist(out[c("rho")])))
    E <- as.numeric(unlist(out[c("E")]))[E]

    plot(out$E, out$rho, type="l", main=colnames(var)[-1], xlab="E", ylab="ρ")
    return(E)
}

# identify the optimal tps
ComputeTp <- function(var, sequence=0:10) {
    opt_e <- ComputeE(var)
    out <- simplex(var, lib=lib, pred=pred, E=opt_e, tp=sequence)
    tp <- which.max(as.numeric(unlist(out[c("rho")])))
    tp <- as.numeric(unlist(out[c("tp")]))[tp]

    plot(out$tp, out$rho, type="l", main=colnames(var)[-1], xlab="tp", ylab="ρ")
    return(tp)
}

# identify nonlinearity
PlotNonlinearity <- function(var) {
    opt_e <- ComputeE(var)
    out <- s_map(var, lib, pred, E=opt_e)
    plot(out$theta, out$rho, type="l", main=colnames(var)[-1], xlab="θ", ylab="ρ")
}

# make and plot predictions
PlotPredictions <- function(var, sequence=0:10) {
    opt_e <- ComputeE(var)
    opt_tp <- ComputeTp(var, sequence)
    out <- simplex(var, lib=lib, pred=pred, E=opt_e, tp=sequence, stats_only=FALSE)
    preds <- na.omit(out$model_output[[opt_tp + 1]])

    plot(var, type="l", main=colnames(var)[-1], xlab="Time", ylab="Value")
    lines(preds$time, preds$pred, col="blue", lty=2)
    polygon(c(preds$time, rev(preds$time)), c(preds$pred - sqrt(preds$pred_var),
            rev(preds$pred + sqrt(preds$pred_var))), col=rgb(0,0,1,0.3), border=NA)
}

# conduct and plot analysis of causality
PlotCausality <- function(vars=list(steering, left, right), tp=-10:10) {
    # identify the optimal embedding dimension (E) for each column
    i <- 1
    opt_e <- list()
    var_names <- list()

    for (var in vars) {
        var_names[i] <- colnames(var)[-1]
        opt_e[i] <- ComputeE(var)
        i <- i + 1
    }

    opt_e <- unlist(opt_e)
    var_names <- unlist(var_names)

    # get every combination of var1 xmap var2
    # add an (E) column that corresponds to the lib column
    params <- expand.grid(lib_column=var_names, target_column=var_names, 
        tp=tp, stringsAsFactors=FALSE)
    params <- params[params$lib_column != params$target_column,]
    rownames(params) <- NULL
    params$E <- as.integer(mapvalues(params$lib_column, var_names, 
        opt_e, warn_missing=FALSE))

    # compute causality
    out <- do.call(rbind, lapply(seq_len(NROW(params)), function(i) {
        ccm(df, E=params$E[i], random_libs=FALSE, lib_sizes=n, 
            lib_column=params$lib_column[i],
            target_column=params$target_column[i], 
            tp=params$tp[i], silent=TRUE)
    }))

    # add a new column
    out$direction <- paste(out$lib_column, "xmap", out$target_column)

    # plot the causalities
    labels <- paste(as.character(round(0.1 * tp, 2)), "(s)")
    ggplot(out, aes(tp, rho, colour=direction)) + geom_line() + geom_point() + 
        geom_vline(xintercept=0, linetype="dashed") + labs(x="tp", y="ρ") +
        scale_x_discrete(limits=tp, labels=labels)
}

# conduct and plot analysis of causality means
PlotCausalityMeans <- function(var1, var2) {
    # compute means
    label1 <- colnames(var1)[-1]
    label2 <- colnames(var2)[-1]
    opt_e <- ComputeE(df[c("Time", label1, label2)])
    var1_xmap_var2_means <- ccm_means(ccm(df, E=opt_e, num_samples=100, 
        lib_column=label1, target_column=label2, lib_sizes=seq(50, 950, by=50), 
        random_libs=TRUE, replace=TRUE))
    var2_xmap_var1_means <- ccm_means(ccm(df, E=opt_e, num_samples=100, 
        lib_column=label2, target_column=label1, lib_sizes=seq(50, 950, by=50), 
        random_libs=TRUE, replace=TRUE))
    
    # compute parallel maxima
    y1 <- pmax(0, var1_xmap_var2_means$rho)
    y2 <- pmax(0, var2_xmap_var1_means$rho)

    # plot the means
    title <- paste(label1, "&", label2)
    limits <- c(min(min(y1), min(y2)), max(max(y1, max(y2))))
    plot(var1_xmap_var2_means$lib_size, y1, type="l", ylim=limits, 
        main=title, xlab="Library Size", ylab="ρ", col="red",)
    lines(var2_xmap_var1_means$lib_size, y2, col="blue")
    legend(x="topleft", col=c("red", "blue"), lwd=1, bty="n", inset=0.02, cex=0.8,
        legend=c(paste(label1, "xmap", label2), paste(label2, "xmap", label1)))
}

# calls all other functions
RunEdm <- function(vars=list(steering, left, right)){
    for (var in vars) {
        PlotNonlinearity(var)
        PlotPredictions(var)
    }

    for (var1 in vars) {
        for (var2 in vars) {
            if (colnames(var1)[-1] != colnames(var2)[-1]) {
                PlotCausalityMeans(var1, var2)
            }
        }
    }

    PlotCausality(vars)
}
