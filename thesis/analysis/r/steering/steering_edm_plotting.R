frame_dir <- "/Users/david/Documents/masters/thesis/analysis/r/steering/frames/"
image_dir <- "/Users/david/Documents/masters/thesis/images/"
# image_dir <- "/Users/david/Desktop/"

margins <- c(3.1, 3, 1.5, 0.5)
mgp_params <- c(2, 0.75, 0)
cex <- 1.3
height <- 3

# ---------------------------------------------------------------------
# embedding dimension
# ---------------------------------------------------------------------

e_steering <- read.csv(paste(frame_dir, "e-steering-wheel-angle.csv", sep=""))
e_left <- read.csv(paste(frame_dir, "e-left-wheel-rpm.csv", sep=""))
e_right <- read.csv(paste(frame_dir, "e-right-wheel-rpm.csv", sep=""))

pdf(paste(image_dir, "SteeringSystemEDMEmbeddingDimension.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_steering, pch=19, ylim=c(y_min, y_max), xaxt="n", main="Steering Wheel Angle", xlab="", ylab="Forecast Skill")
axis(1, at=1:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_left, pch=19, ylim=c(y_min, y_max), main="Left Wheel RPM", xlab="Embedding Dimension", ylab="")
axis(1, at=1:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ E, data=e_right, pch=19, ylim=c(y_min, y_max), main="Right Wheel RPM", xlab="", ylab="")
axis(1, at=1:10)

dev.off()

# ---------------------------------------------------------------------
# deterministic chaos
# ---------------------------------------------------------------------

tp_steering <- read.csv(paste(frame_dir, "tp-steering-wheel-angle.csv", sep=""))
tp_left <- read.csv(paste(frame_dir, "tp-left-wheel-rpm.csv", sep=""))
tp_right <- read.csv(paste(frame_dir, "tp-right-wheel-rpm.csv", sep=""))

pdf(paste(image_dir, "SteeringSystemEDMDeterministicChaos.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_steering, type="l", ylim=c(y_min, y_max), xaxt="n", main="Steering Wheel Angle", xlab="", ylab="Forecast Skill")
axis(1, at=0:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_left, type="l", ylim=c(y_min, y_max), xaxt="n", main="Left Wheel RPM", xlab="Time to Prediction (seconds)", ylab="")
axis(1, at=0:10)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ tp, data=tp_right, type="l", ylim=c(y_min, y_max), xaxt="n", main="Right Wheel RPM", xlab="", ylab="")
axis(1, at=0:10)

dev.off()

# ---------------------------------------------------------------------
# nonlinearity
# ---------------------------------------------------------------------

nonlinear_steering <- read.csv(paste(frame_dir, "smap-steering-wheel-angle.csv", sep=""))
nonlinear_left <- read.csv(paste(frame_dir, "smap-left-wheel-rpm.csv", sep=""))
nonlinear_right <- read.csv(paste(frame_dir, "smap-right-wheel-rpm.csv", sep=""))

pdf(paste(image_dir, "SteeringSystemEDMNonlinearity.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))
rounding <- 2
top_tick_location <- 4/5
y_min <- 0
y_max <- 1

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_steering, type="l", ylim=c(y_min, y_max), xaxt="n", main="Steering Wheel Angle", xlab="", ylab="Forecast Skill")
axis(1, at=0:8)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_left, type="l", ylim=c(y_min, y_max), xaxt="n", main="Left Wheel RPM", xlab="Nonlinearity Tuning Parameter", ylab="")
axis(1, at=0:8)

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(rho ~ theta, data=nonlinear_right, type="l", ylim=c(y_min, y_max), xaxt="n", main="Right Wheel RPM", xlab="", ylab="")
axis(1, at=0:8)

dev.off()

# ---------------------------------------------------------------------
# predictions
# ---------------------------------------------------------------------

df <- read.csv("/Users/david/Documents/masters/thesis/data/steering/steering.csv")
steering <- df[c("Time", "Steering.wheel.angle")]
left <- df[c("Time", "Left.wheel.RPM")]
right <- df[c("Time", "Right.wheel.RPM")]

steering$Steering.wheel.angle <- scale(steering$Steering.wheel.angle)
left$Left.wheel.RPM <- scale(left$Left.wheel.RPM)
right$Right.wheel.RPM <- scale(right$Right.wheel.RPM)

predictions_steering <- read.csv(paste(frame_dir, "predictions-steering-wheel-angle.csv", sep=""))
predictions_left <- read.csv(paste(frame_dir, "predictions-left-wheel-rpm.csv", sep=""))
predictions_right <- read.csv(paste(frame_dir, "predictions-right-wheel-rpm.csv", sep=""))

files <- c("SteeringSystemColorEDMPredictions.pdf", "SteeringSystemBWEDMPredictions.pdf")
colors <- c(rgb(0, 0, 0, 1), rgb(1, 0, 0, 1), rgb(1, 0, 0, 3/4),
            rgb(2/3, 2/3, 2/3, 1), rgb(0, 0, 0, 1), rgb(2/3, 2/3, 2/3, 3/4))
ltypes <- c("55", "12")

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=height)
    layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(steering, type="l", main="Steering Wheel Angle", xlab="", ylab="Angle (degrees)", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_steering, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_steering$time, rev(predictions_steering$time)), c(predictions_steering$pred - sqrt(predictions_steering$pred_var),
            rev(predictions_steering$pred + sqrt(predictions_steering$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)
    legend(x="bottomright", col=colors[(i - 1) * 3 + 1:2], bty="n", lwd=1, inset=0.02, cex=cex,
        legend=c("Observed", "Predicted"), lty=ltypes)

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(left, type="l", main="Left Wheel RPM", xlab="Time (seconds)", ylab="RPM", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_left, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_left$time, rev(predictions_left$time)), c(predictions_left$pred - sqrt(predictions_left$pred_var),
            rev(predictions_left$pred + sqrt(predictions_left$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)

    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(right, type="l", main="Right Wheel RPM", xlab="", ylab="RPM", col=colors[(i - 1) * 3 + 1], lty=ltypes[1])
    lines(pred ~ time, data=predictions_right, col=colors[(i - 1) * 3 + 2], lty=ltypes[2])
    polygon(c(predictions_right$time, rev(predictions_right$time)), c(predictions_right$pred - sqrt(predictions_right$pred_var),
            rev(predictions_right$pred + sqrt(predictions_right$pred_var))), col=colors[(i - 1) * 3 + 3], border=NA)

    dev.off()
}

# ---------------------------------------------------------------------
# prediction residuals
# ---------------------------------------------------------------------

pdf(paste(image_dir, "SteeringSystemEDMPredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

diff_steering <- abs(predictions_steering$pred - predictions_steering$obs)
diff_left <- abs(predictions_left$pred - predictions_left$obs)
diff_right <- abs(predictions_right$pred - predictions_right$obs)
y_min <- 0
y_max <- max(diff_steering, diff_left, diff_right)

rmse_steering <- sqrt(mean((predictions_steering$pred - predictions_steering$obs)^2))
rmse_left <- sqrt(mean((predictions_left$pred - predictions_left$obs)^2))
rmse_right <- sqrt(mean((predictions_right$pred - predictions_right$obs)^2))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_steering ~ predictions_steering$time, ylim=c(y_min, y_max), type="l", main="Steering Wheel Angle", xlab="", ylab="Error (degrees)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_left ~ predictions_left$time, ylim=c(y_min, y_max), type="l", main="Left Wheel RPM", xlab="Time (seconds)", ylab="Error (RPM)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_right ~ predictions_right$time, ylim=c(y_min, y_max), type="l", main="Right Wheel RPM", xlab="", ylab="Error (RPM)")

dev.off()

# ---------------------------------------------------------------------
# naive prediction residuals
# ---------------------------------------------------------------------

df <- read.csv("/Users/david/Documents/masters/thesis/data/steering/steering.csv")
steering_obs <- tail(df$Steering, length(predictions_steering$pred) + 1)
left_obs <- tail(df$Left, length(predictions_left$pred) + 1)
right_obs <- tail(df$Right, length(predictions_right$pred) + 1)

pdf(paste(image_dir, "SteeringSystemNaivePredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

diff_naive_steering <- abs(head(steering_obs, -1) - tail(steering_obs, -1))
diff_naive_left <- abs(head(left_obs, -1) - tail(left_obs, -1))
diff_naive_right <- abs(head(right_obs, -1) - tail(right_obs, -1))
y_min <- 0
y_max <- max(diff_naive_steering, diff_naive_left, diff_naive_right)

rmse_naive_steering <- sqrt(mean((head(steering_obs, -1) - tail(steering_obs, -1))^2))
rmse_naive_left <- sqrt(mean((head(left_obs, -1) - tail(left_obs, -1))^2))
rmse_naive_right <- sqrt(mean((head(right_obs, -1) - tail(right_obs, -1))^2))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_steering ~ predictions_steering$time, ylim=c(y_min, y_max), type="l", main="Steering Wheel Angle", xlab="", ylab="Error (degrees)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_left ~ predictions_left$time, ylim=c(y_min, y_max), type="l", main="Left Wheel RPM", xlab="Time (seconds)", ylab="Error (RPM)")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_naive_right ~ predictions_right$time, ylim=c(y_min, y_max), type="l", main="Right Wheel RPM", xlab="", ylab="Error (RPM)")

dev.off()

# ---------------------------------------------------------------------
# edm and naive prediction residuals
# ---------------------------------------------------------------------

pdf(paste(image_dir, "SteeringSystemEDMAllPredictionResiduals.pdf", sep=""), width=9, height=height)
layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

y_min <- 0
y_max <- max(diff_steering, diff_left, diff_right, diff_naive_steering, diff_naive_left, diff_naive_right)
colors <- c(rgb(1, 0, 0, 1), rgb(0, 0, 0, 1))
ltypes <- c("12", "55")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_steering ~ predictions_steering$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Steering Wheel Angle", xlab="", ylab="Error (feet per second)")
lines(diff_naive_steering ~ predictions_steering$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topright", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_left ~ predictions_left$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Left Wheel RPM", xlab="Time (seconds)", ylab="Error (feet)")
lines(diff_naive_left ~ predictions_left$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topright", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(diff_right ~ predictions_right$time, ylim=c(y_min, y_max), type="l", 
    lty=ltypes[1], col=colors[1], main="Right Wheel RPM", xlab="", ylab="Error (degrees)")
lines(diff_naive_right ~ predictions_right$time, ylim=c(y_min, y_max), type="l", lty=ltypes[2], col=colors[2])
legend(x="topright", col=colors, lty=ltypes, bty="n", lwd=1, inset=0.02, cex=cex, legend=c("EDM Predictor", "Naive Predictor"))

dev.off()

# ---------------------------------------------------------------------
# causality means
# ---------------------------------------------------------------------

steering_x_left <- read.csv(paste(frame_dir, "xmap-steering-wheel-angle-and-left-wheel-rpm.csv", sep=""))
left_x_steering <- read.csv(paste(frame_dir, "xmap-left-wheel-rpm-and-steering-wheel-angle.csv", sep=""))
steering_x_right <- read.csv(paste(frame_dir, "xmap-steering-wheel-angle-and-right-wheel-rpm.csv", sep=""))
right_x_steering <- read.csv(paste(frame_dir, "xmap-right-wheel-rpm-and-steering-wheel-angle.csv", sep=""))
left_x_right <- read.csv(paste(frame_dir, "xmap-left-wheel-rpm-and-right-wheel-rpm.csv", sep=""))
right_x_left <- read.csv(paste(frame_dir, "xmap-right-wheel-rpm-and-left-wheel-rpm.csv", sep=""))

files <- c("SteeringSystemColorEDMCausalityMeans.pdf", "SteeringSystemBWEDMCausalityMeans.pdf")
colors <- c(rgb(0, 0, 0, 1), rgb(1, 0, 0, 1),
            rgb(2/3, 2/3, 2/3, 1), rgb(0, 0, 0, 1))
ltypes <- c("55", "12")
means_margins <- c(3, 3, 2.5, 0.5)
y_min <- 0
y_max <- 1

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=height)
    layout(matrix(c(1,2,3), 1, 3, byrow=TRUE))

    y1 <- pmax(0, steering_x_left$rho)
    y2 <- pmax(0, left_x_steering$rho)
    title <- "Steering Wheel Angle\n& Left Wheel RPM"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(steering_x_left$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="", ylab="Cross-Map Skill")
    lines(left_x_steering$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Steering xmap Left RPM", "Left RPM xmap Steering"), lty=ltypes)

    y1 <- pmax(0, steering_x_right$rho)
    y2 <- pmax(0, right_x_steering$rho)
    title <- "Steering Wheel Angle\n& Right Wheel RPM"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(steering_x_right$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="Library Size", ylab="")
    lines(right_x_steering$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Steering xmap Right RPM", "Right RPM xmap Steering"), lty=ltypes)

    y1 <- pmax(0, left_x_right$rho)
    y2 <- pmax(0, right_x_left$rho)
    title <- "Left Wheel RPM\n& Right Wheel RPM"
    par(mar=means_margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(left_x_right$lib_size, y1, type="l", lty=ltypes[1], col=colors[(i - 1) * 2 + 1], ylim=c(y_min, y_max), main=title, xlab="", ylab="")
    lines(right_x_left$lib_size, y2, lty=ltypes[2], col=colors[(i - 1) * 2 + 2])
    legend(x="bottomright", col=colors[(i - 1) * 2 + 1:2], lwd=1, bty="n", inset=0.02, cex=1.1, legend=c("Left RPM xmap Right RPM", "Right RPM xmap Left RPM"), lty=ltypes)

    dev.off()
}

# ---------------------------------------------------------------------
# causality predictions
# ---------------------------------------------------------------------

library(stringr)
library(scales)

causality <- read.csv(paste(frame_dir, "causality.csv", sep=""))
causality$direction <- str_replace_all(causality$direction,
                                       c("Left.wheel.RPM" = "Left RPM", "Right.wheel.RPM" = "Right RPM", "Steering.wheel.angle" = "Steering"))
directions <- unique(causality$direction)
y_min <- 0
y_max <- 1

cex <- 0.8
files <- c("SteeringSystemColorEDMCausalityPredictions.pdf", "SteeringSystemBWEDMCausalityPredictions.pdf")
colors <- c(hue_pal()(length(directions)),
            rgb(0/6, 0/6, 0/6, 1), rgb(1/6, 1/6, 1/6, 1), rgb(2/6, 2/6, 2/6, 1),
            rgb(3/6, 3/6, 3/6, 1), rgb(4/6, 4/6, 4/6, 1), rgb(5/6, 5/6, 5/6, 1))
ltypes <- c("11", "71", "222282", "22F2", "46", "22222242")

for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=3)
    par(mar=c(2.3, 2.2, 1, 9), mgp=c(1.35, 0.5, 0), cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(rho ~ tp, causality[causality[, "direction"] == directions[1],], lty=ltypes[1],
        ylim=c(y_min, y_max), col=colors[(i - 1) * 6 + 1], cex=0.5, type="l", main="", 
        xlab="Time to Prediction (seconds)", ylab="Cross-Map Skill")

    for (j in 2:length(directions)) {
        lines(rho ~ tp, data=causality[causality[, "direction"] == directions[j],], 
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
# cat("Steering wheel angle & ", formatC(rmse_naive_steering, digits=rounding, format="f"), " & ", formatC(rmse_steering, digits=rounding, format="f"), " \\\\", "\n",
#     "Left wheel RPM & ", formatC(rmse_naive_left, digits=rounding, format="f"), " & ", formatC(rmse_left, digits=rounding, format="f"), " \\\\", "\n",
#     "Right wheel RPM & ", formatC(rmse_naive_right, digits=rounding, format="f"), " & ", formatC(rmse_right, digits=rounding, format="f"), " \\\\", sep="")
