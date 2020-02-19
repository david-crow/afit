frame_dir <- "/Users/david/Documents/masters/thesis/analysis/r/avas/frames/"
# image_dir <- "/Users/david/Documents/masters/thesis/images/"
image_dir <- "/Users/david/Desktop/"

margins <- c(3.1, 3, 1.5, 0.5)
mgp_params <- c(2, 0.75, 0)
cex <- 1.3
height <- 3

# ---------------------------------------------------------------------
# embedding dimension
# ---------------------------------------------------------------------

e_airspeed <- read.csv(paste(frame_dir, "e-airspeed.csv", sep=""))
e_altitude <- read.csv(paste(frame_dir, "e-altitude.csv", sep=""))
e_pitch <- read.csv(paste(frame_dir, "e-pitch.csv", sep=""))

pdf(paste(image_dir, "AVASEDMEmbeddingDimension.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_airspeed, pch=19, ylim=c(y_min, y_max), main="Airspeed", xlab="", ylab="Forecast Skill")
axis(1, at=1:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_altitude, pch=19, ylim=c(y_min, y_max), main="Altitude", xlab="Embedding Dimension", ylab="")
axis(1, at=1:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_pitch, pch=19, ylim=c(y_min, y_max), main="Pitch", xlab="", ylab="")
axis(1, at=1:10)

dev.off()

# ---------------------------------------------------------------------
# deterministic chaos
# ---------------------------------------------------------------------

tp_airspeed <- read.csv(paste(frame_dir, "tp-airspeed.csv", sep=""))
tp_altitude <- read.csv(paste(frame_dir, "tp-altitude.csv", sep=""))
tp_pitch <- read.csv(paste(frame_dir, "tp-pitch.csv", sep=""))

pdf(paste(image_dir, "AVASEDMDeterministicChaos.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
sequence <- seq(0,100,10)
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_airspeed, type="l", ylim=c(y_min, y_max), xaxt="n", main="Airspeed", xlab="", ylab="Forecast Skill")
axis(1, at=sequence, labels=0.1 * sequence)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_altitude, type="l", ylim=c(y_min, y_max), xaxt="n", main="Altitude", xlab="Time to Prediction (seconds)", ylab="")
axis(1, at=sequence, labels=0.1 * sequence)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_pitch, type="l", ylim=c(y_min, y_max), xaxt="n", main="Pitch", xlab="", ylab="")
axis(1, at=sequence, labels=0.1 * sequence)

dev.off()

# ---------------------------------------------------------------------
# nonlinearity
# ---------------------------------------------------------------------

nonlinear_airspeed <- read.csv(paste(frame_dir, "smap-airspeed.csv", sep=""))
nonlinear_altitude <- read.csv(paste(frame_dir, "smap-altitude.csv", sep=""))
nonlinear_pitch <- read.csv(paste(frame_dir, "smap-pitch.csv", sep=""))

pdf(paste(image_dir, "AVASEDMNonlinearity.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_airspeed, type="l", ylim=c(y_min, y_max), xaxt="n", main="Airspeed", xlab="", ylab="Forecast Skill")
axis(1, at=0:8)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_altitude, type="l", ylim=c(y_min, y_max), xaxt="n", main="Altitude", xlab="Nonlinearity Tuning Parameter", ylab="")
axis(1, at=0:8)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_pitch, type="l", ylim=c(y_min, y_max), xaxt="n", main="Pitch", xlab="", ylab="")
axis(1, at=0:8)

dev.off()

# ---------------------------------------------------------------------
# predictions
# ---------------------------------------------------------------------

df <- read.csv("/Users/david/Documents/masters/thesis/data/avas/edm/avas8.csv")
altitude <- df[c("Time", "Altitude")]
airspeed <- df[c("Time", "Airspeed")]
pitch <- df[c("Time", "Pitch")]

pitch$Pitch <- pitch$Pitch * 180 / pi

altitude$Altitude <- scale(altitude$Altitude)
airspeed$Airspeed <- scale(airspeed$Airspeed)
pitch$Pitch <- scale(pitch$Pitch)

predictions_airspeed <- read.csv(paste(frame_dir, "predictions-airspeed.csv", sep=""))
predictions_altitude <- read.csv(paste(frame_dir, "predictions-altitude.csv", sep=""))
predictions_pitch <- read.csv(paste(frame_dir, "predictions-pitch.csv", sep=""))

files <- c("AVASColorEDMPredictions.pdf", "AVASBWEDMPredictions.pdf")
colors <- c(rgb(0, 0, 0, 1), rgb(1, 0, 0, 1), rgb(1, 0, 0, 3/4),
            rgb(2/3, 2/3, 2/3, 1), rgb(0, 0, 0, 2/3), rgb(2/3, 2/3, 2/3, 3/4))
ltypes <- c("55", "12")

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=height)
    layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(airspeed, type="l", main="Airspeed", xlab="", ylab="Speed (feet per second)", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_airspeed, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_airspeed$time, rev(predictions_airspeed$time)), c(predictions_airspeed$pred - sqrt(predictions_airspeed$pred_var),
            rev(predictions_airspeed$pred + sqrt(predictions_airspeed$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)
    legend(x="bottomright", col=colors[(i - 1) * 3 + 1:2], bty="n", lwd=1, inset=0.02, cex=cex,
        legend=c("Observed", "Predicted"), lty=ltypes)

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(altitude, type="l", main="Altitude", xlab="Time (seconds)", ylab="Altitude (feet)", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_altitude, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_altitude$time, rev(predictions_altitude$time)), c(predictions_altitude$pred - sqrt(predictions_altitude$pred_var),
            rev(predictions_altitude$pred + sqrt(predictions_altitude$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(pitch, type="l", main="Pitch", xlab="", ylab="Angle (degrees)", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_pitch, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_pitch$time, rev(predictions_pitch$time)), c(predictions_pitch$pred - sqrt(predictions_pitch$pred_var),
            rev(predictions_pitch$pred + sqrt(predictions_pitch$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)

    dev.off()
}

# ---------------------------------------------------------------------
# prediction residuals
# ---------------------------------------------------------------------

pdf(paste(image_dir, "AVASEDMPredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

diff_airspeed <- abs(predictions_airspeed$pred - predictions_airspeed$obs)
diff_altitude <- abs(predictions_altitude$pred - predictions_altitude$obs)
diff_pitch <- abs(predictions_pitch$pred - predictions_pitch$obs)
y_min <- 0
y_max <- max(diff_airspeed, diff_altitude, diff_pitch)

rmse_airspeed <- sqrt(mean((predictions_airspeed$pred - predictions_airspeed$obs)^2))
rmse_altitude <- sqrt(mean((predictions_altitude$pred - predictions_altitude$obs)^2))
rmse_pitch <- sqrt(mean((predictions_pitch$pred - predictions_pitch$obs)^2))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_airspeed ~ predictions_airspeed$time, ylim=c(y_min, y_max), type="l", main="Airspeed", xlab="", ylab="Error (feet per second)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_altitude ~ predictions_altitude$time, ylim=c(y_min, y_max), type="l", main="Altitude", xlab="Time (seconds)", ylab="Error (feet)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_pitch ~ predictions_pitch$time, ylim=c(y_min, y_max), type="l", main="Pitch", xlab="", ylab="Error (degrees)")

dev.off()

# ---------------------------------------------------------------------
# naive prediction residuals
# ---------------------------------------------------------------------

df <- read.csv("/Users/david/Documents/masters/thesis/data/avas/edm/avas8.csv")
airspeed_obs <- tail(df$Airspeed, length(predictions_airspeed$pred) + 1)
altitude_obs <- tail(df$Altitude, length(predictions_altitude$pred) + 1)
pitch_obs <- tail(df$Pitch, length(predictions_pitch$pred) + 1)

pdf(paste(image_dir, "AVASNaivePredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

diff_naive_airspeed <- abs(head(airspeed_obs, -1) - tail(airspeed_obs, -1))
diff_naive_altitude <- abs(head(altitude_obs, -1) - tail(altitude_obs, -1))
diff_naive_pitch <- abs(head(pitch_obs, -1) - tail(pitch_obs, -1))
y_min <- 0
y_max <- max(diff_naive_airspeed, diff_naive_altitude, diff_naive_pitch)

rmse_naive_airspeed <- sqrt(mean((head(airspeed_obs, -1) - tail(airspeed_obs, -1))^2))
rmse_naive_altitude <- sqrt(mean((head(altitude_obs, -1) - tail(altitude_obs, -1))^2))
rmse_naive_pitch <- sqrt(mean((head(pitch_obs, -1) - tail(pitch_obs, -1))^2))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_airspeed ~ predictions_airspeed$time, ylim=c(y_min, y_max), 
    type="l", main="Airspeed", xlab="", ylab="Error (feet per second)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_altitude ~ predictions_altitude$time, ylim=c(y_min, y_max), 
    type="l", main="Altitude", xlab="Time (seconds)", ylab="Error (feet)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_pitch ~ predictions_pitch$time, ylim=c(y_min, y_max), 
    type="l", main="Pitch", xlab="", ylab="Error (degrees)")

dev.off()

# ---------------------------------------------------------------------
# edm and naive prediction residuals
# ---------------------------------------------------------------------

pdf(paste(image_dir, "AVASEDMAllPredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

y_min <- 0
y_max <- max(diff_airspeed, diff_altitude, diff_pitch, diff_naive_airspeed, diff_naive_altitude, diff_naive_pitch)
colors <- c(rgb(1, 0, 0, 1), rgb(0, 0, 0, 1))
ltypes <- c("12", "55")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_airspeed ~ predictions_airspeed$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Airspeed", xlab="", ylab="Error (feet per second)")
lines(diff_naive_airspeed ~ predictions_airspeed$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topleft", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_altitude ~ predictions_altitude$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Altitude", xlab="Time (seconds)", ylab="Error (feet)")
lines(diff_naive_altitude ~ predictions_altitude$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topleft", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_pitch ~ predictions_pitch$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Pitch", xlab="", ylab="Error (degrees)")
lines(diff_naive_pitch ~ predictions_pitch$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topleft", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

dev.off()

# ---------------------------------------------------------------------
# causality means
# ---------------------------------------------------------------------

airspeed_x_altitude <- read.csv(paste(frame_dir, "xmap-airspeed-and-altitude.csv", sep=""))
altitude_x_airspeed <- read.csv(paste(frame_dir, "xmap-altitude-and-airspeed.csv", sep=""))
airspeed_x_pitch <- read.csv(paste(frame_dir, "xmap-airspeed-and-pitch.csv", sep=""))
pitch_x_airspeed <- read.csv(paste(frame_dir, "xmap-pitch-and-airspeed.csv", sep=""))
altitude_x_pitch <- read.csv(paste(frame_dir, "xmap-altitude-and-pitch.csv", sep=""))
pitch_x_altitude <- read.csv(paste(frame_dir, "xmap-pitch-and-altitude.csv", sep=""))

files <- c("AVASColorEDMCausalityMeans.pdf", "AVASBWEDMCausalityMeans.pdf")
colors <- c(rgb(0, 0, 0, 1), rgb(1, 0, 0, 1),
            rgb(2/3, 2/3, 2/3, 1), rgb(0, 0, 0, 1))
ltypes <- c("55", "12")
means_margins <- c(3, 3, 2.5, 0.5)
y_min <- 0
y_max <- 1

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=height)
    layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

    y1 <- pmax(0, airspeed_x_altitude$rho)
    y2 <- pmax(0, altitude_x_airspeed$rho)
    title <- "Airspeed\n& Altitude"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(airspeed_x_altitude$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="", ylab="Cross-Map Skill")
    lines(altitude_x_airspeed$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Airspeed xmap Altitude", "Altitude xmap Airspeed"), lty=ltypes)

    y1 <- pmax(0, airspeed_x_pitch$rho)
    y2 <- pmax(0, pitch_x_airspeed$rho)
    title <- "Airspeed\n& Pitch"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(airspeed_x_pitch$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="Library Size", ylab="")
    lines(pitch_x_airspeed$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Airspeed xmap Pitch", "Pitch xmap Airspeed"), lty=ltypes)

    y1 <- pmax(0, altitude_x_pitch$rho)
    y2 <- pmax(0, pitch_x_altitude$rho)
    title <- "Altitude\n& Pitch"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(altitude_x_pitch$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="", ylab="")
    lines(pitch_x_altitude$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Altitude xmap Pitch", "Pitch xmap Altitude"), lty=ltypes)

    dev.off()
}

# ---------------------------------------------------------------------
# causality predictions
# ---------------------------------------------------------------------

library(stringr)
library(scales)

causality <- read.csv(paste(frame_dir, "causality.csv", sep=""))
causality$tp <- causality$tp * 0.1
directions <- unique(causality$direction)
y_min <- 0
y_max <- 1

cex <- 0.8
files <- c("AVASColorEDMCausalityPredictions.pdf", "AVASBWEDMCausalityPredictions.pdf")
colors <- c(hue_pal()(length(directions)),
            rgb(0/6, 0/6, 0/6, 1), rgb(1/6, 1/6, 1/6, 1), rgb(2/6, 2/6, 2/6, 1),
            rgb(3/6, 3/6, 3/6, 1), rgb(4/6, 4/6, 4/6, 1), rgb(5/6, 5/6, 5/6, 1))
ltypes <- c("11", "71", "222282", "22F2", "46", "22222242")

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=3)
    par(mar=c(2.3, 2.2, 1, 8), mgp=c(1.35, 0.5, 0), cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(rho ~ tp, causality[causality[, "direction"] == directions[1],], lty=ltypes[1],
        ylim=c(y_min, y_max), col=colors[(i - 1) * 6 + 1], cex=0.5, type="l", main="", 
        xlab="Time to Prediction (seconds)", ylab="Cross-Map Skill")

    for (j in 2:length(directions)) {
        lines(rho ~ tp, causality[causality[, "direction"] == directions[j],], 
            col=colors[(i - 1) * 6 + j], type="l", lty=ltypes[j])
    }

    legend(x=par("usr")[2], y=y_max - 0.2 * (y_max - y_min), bty="n", col=colors[(i - 1) * 6 + 1:6], 
        lwd=1, inset=0.02, cex=0.7, legend=directions, xpd=TRUE, lty=ltypes)
    abline(v=0, lty="solid")
    text(x=par("usr")[1] / 2, y=y_max + 0.10 * (y_max - y_min), labels="Y causes X", cex=cex, xpd=TRUE)
    text(x=par("usr")[2] / 2, y=y_max + 0.10 * (y_max - y_min), labels="X causes Y", cex=cex, xpd=TRUE)

    dev.off()
}

# rounding <- 6
# cat("Airspeed & ", formatC(rmse_naive_airspeed, digits=rounding, format="f"), " & ", formatC(rmse_airspeed, digits=rounding, format="f"), " \\\\", "\n",
#     "Altitude & ", formatC(rmse_naive_altitude, digits=rounding, format="f"), " & ", formatC(rmse_altitude, digits=rounding, format="f"), " \\\\", "\n",
#     "Pitch & ", formatC(rmse_naive_pitch, digits=rounding, format="f"), " & ", formatC(rmse_pitch, digits=rounding, format="f"), " \\\\", sep="")
