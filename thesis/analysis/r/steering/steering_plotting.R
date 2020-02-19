library(scales)
df <- read.csv("/Users/david/Documents/masters/thesis/data/steering/steering.csv")
image_dir <- "/Users/david/Documents/masters/thesis/images/"

# parameters
mgp_params <- c(2, 0.75, 0)
cex <- 1.3
height <- 3

# ---------------------------------------------------------------------
# relationships
# ---------------------------------------------------------------------

pdf(paste(image_dir, "SteeringSystemLinearRelationships.pdf", sep=""), width=9, height=height)
layout(matrix(1:3, 1, 3, byrow=TRUE))
margins <- c(3.2, 3.5, 0.7, 0.5)

xcols <- c("Left.wheel.RPM", "Steering.wheel.angle", "Steering.wheel.angle")
ycols <- c("Right.wheel.RPM", "Left.wheel.RPM", "Right.wheel.RPM")
xlabs <- c("Left RPM", "Steering Wheel Angle (degrees)", "Steering Wheel Angle (degrees)")
ylabs <- c("Right RPM", "Left RPM", "Right RPM")

for (i in 1:3) {
    x <- df[[xcols[i]]]
    y <- df[[ycols[i]]]
    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
    plot(y ~ x, cex=0.5, xlab=xlabs[i], ylab=ylabs[i], xaxt="n", yaxt="n")
    axis(1, at=round(seq(floor(min(x)), ceiling(max(x)), l=5)))
    axis(2, at=round(seq(floor(min(y)), ceiling(max(y)), l=3)))
}

dev.off()

# ---------------------------------------------------------------------
# time series (scaled -- black & white)
# ---------------------------------------------------------------------

pdf(paste(image_dir, "SteeringSystemBWTimeSeriesScaled.pdf", sep=""), width=9, height=3)
cex <- 0.8
margins <- c(2.3, 5, 0.2, 6)
mgp_params <- c(1.3, 0.5, 0)

min_wheel_rpm <- min(df$Left.wheel.RPM)
max_wheel_rpm <- max(df$Left.wheel.RPM)

df$Left.wheel.RPM <- ((df$Left.wheel.RPM - min(df$Left.wheel.RPM)) / (max(df$Left.wheel.RPM) - min(df$Left.wheel.RPM)))
df$Right.wheel.RPM <- ((df$Right.wheel.RPM - min(df$Right.wheel.RPM)) / (max(df$Right.wheel.RPM) - min(df$Right.wheel.RPM)))
df$Steering.wheel.angle <- ((df$Steering.wheel.angle - min(df$Steering.wheel.angle)) / (max(df$Steering.wheel.angle) - min(df$Steering.wheel.angle)))
df$Steering.wheel.angle <- df$Steering.wheel.angle * 2 - 1

labels <- c("Steering", "Left RPM", "Right RPM")
colors <- c(rgb(0, 0, 0, 0.9), rgb(0, 0, 0, 0.6), rgb(0, 0, 0, 0.4))
ltypes <- c("2424F4", "84", "22")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Steering.wheel.angle ~ Time, data=df, type="l", lty=ltypes[1], ylim=c(-1, 1), yaxt="n", col=colors[1], xlab="Time (seconds)", ylab="")
axis(2, at=seq(-1, 1, length.out=5), labels=seq(-360, 360, length.out=5), line=2.7)
mtext("Steering Wheel Angle (degrees)", side=2, line=4.2, at=0, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Left.wheel.RPM ~ Time, data=df, type="l", lty=ltypes[2], ylim=c(-1, 1), yaxt="n", col=colors[2], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_wheel_rpm, max_wheel_rpm, length.out=3)))
mtext("Wheel RPM", side=2, line=1.2, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Right.wheel.RPM ~ Time, data=df, type="l", lty=ltypes[3], ylim=c(-1, 1), yaxt="n", col=colors[3], xlab="Time (seconds)", ylab="")

abline(h=0, lty="solid", col="black")
legend(x=par("usr")[2], y=0.8, bty="n", col=colors, 
       lwd=1, inset=0.02, cex=cex, legend=labels, xpd=TRUE, lty=ltypes)

dev.off()

plot((Left.wheel.RPM - Right.wheel.RPM) ~ Steering.wheel.angle, data=df)

# ---------------------------------------------------------------------
# time series (scaled -- color)
# ---------------------------------------------------------------------

pdf(paste(image_dir, "SteeringSystemColorTimeSeriesScaled.pdf", sep=""), width=9, height=3)
colors <- hue_pal()(length(labels))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Steering.wheel.angle ~ Time, data=df, type="l", lty=ltypes[1], ylim=c(-1, 1), yaxt="n", col=colors[1], xlab="Time (seconds)", ylab="")
axis(2, at=seq(-1, 1, length.out=5), labels=seq(-360, 360, length.out=5), line=2.7)
mtext("Steering Wheel Angle (degrees)", side=2, line=4.2, at=0, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Left.wheel.RPM ~ Time, data=df, type="l", lty=ltypes[2], ylim=c(-1, 1), yaxt="n", col=colors[2], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_wheel_rpm, max_wheel_rpm, length.out=3)))
mtext("Wheel RPM", side=2, line=1.2, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Right.wheel.RPM ~ Time, data=df, type="l", lty=ltypes[3], ylim=c(-1, 1), yaxt="n", col=colors[3], xlab="Time (seconds)", ylab="")

abline(h=0, lty="solid", col="black")
legend(x=par("usr")[2], y=0.8, bty="n", col=colors, 
       lwd=1, inset=0.02, cex=cex, legend=labels, xpd=TRUE, lty=ltypes)

dev.off()
