library(scales)
df <- read.csv("/Users/david/Documents/masters/thesis/data/avas/edm/avas8.csv")
# image_dir <- "/Users/david/Documents/masters/thesis/images/"
image_dir <- "/Users/david/Desktop/"

# parameters
mgp_params <- c(2.5, 1, 0)
cex <- 1.3
height <- 2

# convert radians to degrees
for (l in c("Roll", "Pitch")) {
    df[l] <- df[l] * 180 / pi
}

# fix the negative series
for (l in c("Vertical.Velocity", "Y.Acceleration")) {
    df[l] <- df[l] * -1
}

# ---------------------------------------------------------------------
# relationships
# ---------------------------------------------------------------------

pdf(paste(image_dir, "AVASNonlinearRelationships.pdf", sep=""), width=9, height=height)
layout(matrix(1:3, 1, 3, byrow=TRUE))
margins <- c(3.5, 3.9, 1.3, 1)

xcols <- c("Airspeed", "Airspeed", "Altitude")
ycols <- c("Altitude", "Pitch", "Pitch")
xlabs <- c("Airspeed (feet/second)", "Airspeed (feet/second)", "Altitude (feet)")
ylabs <- c("Altitude (feet)", "Pitch (degrees)", "Pitch (degrees)")

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

pdf(paste(image_dir, "AVASBWTimeSeriesScaled.pdf", sep=""), width=9, height=3)
cex <- 0.8
margins <- c(2.3, 7.8, 0.8, 5.7)
mgp_params <- c(1.3, 0.5, 0)

min_airspeed <- min(df$Airspeed)
max_airspeed <- max(df$Airspeed)
min_altitude <- min(df$Altitude)
max_altitude <- max(df$Altitude)
min_pitch <- min(df$Pitch)
max_pitch <- max(df$Pitch)

df$Airspeed <- ((df$Airspeed - min(df$Airspeed)) / (max(df$Airspeed) - min(df$Airspeed)))
df$Altitude <- ((df$Altitude - min(df$Altitude)) / (max(df$Altitude) - min(df$Altitude)))
df$Pitch <- ((df$Pitch + max(df$Pitch)) / (2 * max(df$Pitch)))
df$Pitch <- df$Pitch * 2 - 1

labels <- c("Airspeed", "Altitude", "Pitch")
colors <- c(rgb(0, 0, 0, 0.9), rgb(0, 0, 0, 0.6), rgb(0, 0, 0, 0.4))
ltypes <- c("2424F4", "84", "22")

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Airspeed ~ Time, data=df, type="l", lty=ltypes[1], ylim=c(-1, 1), yaxt="n", col=colors[1], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_airspeed, max_airspeed, length.out=3)))
mtext("Airspeed (feet/second)", side=2, line=1.5, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Altitude ~ Time, data=df, type="l", lty=ltypes[2], ylim=c(-1, 1), yaxt="n", col=colors[2], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_altitude, max_altitude, length.out=3)), line=2.7)
mtext("Altitude (feet)", side=2, line=4.2, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Pitch ~ Time, data=df, type="l", lty=ltypes[3], ylim=c(-1, 1), yaxt="n", col=colors[3], xlab="Time (seconds)", ylab="")
axis(2, at=seq(-1, 1, length.out=5), labels=round(seq(min_pitch, max_pitch, length.out=5)), line=5.4)
mtext("Pitch (degrees)", side=2, line=6.9, at=0, cex=cex)

abline(h=0, lty="solid", col="black")
legend(x=par("usr")[2], y=0.8, bty="n", col=colors, 
       lwd=1, inset=0.02, cex=cex, legend=labels, xpd=TRUE, lty=ltypes)

dev.off()

# ---------------------------------------------------------------------
# time series (scaled -- color)
# ---------------------------------------------------------------------

pdf(paste(image_dir, "AVASColorTimeSeriesScaled.pdf", sep=""), width=9, height=3)
colors <- hue_pal()(length(labels))

par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Airspeed ~ Time, data=df, type="l", lty=ltypes[1], ylim=c(-1, 1), yaxt="n", col=colors[1], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_airspeed, max_airspeed, length.out=3)))
mtext("Airspeed (feet/second)", side=2, line=1.5, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Altitude ~ Time, data=df, type="l", lty=ltypes[2], ylim=c(-1, 1), yaxt="n", col=colors[2], xlab="Time (seconds)", ylab="")
axis(2, at=seq(0, 1, length.out=3), labels=round(seq(min_altitude, max_altitude, length.out=3)), line=2.7)
mtext("Altitude (feet)", side=2, line=4.2, at=0.5, cex=cex)

par(new=TRUE, mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)
plot(Pitch ~ Time, data=df, type="l", lty=ltypes[3], ylim=c(-1, 1), yaxt="n", col=colors[3], xlab="Time (seconds)", ylab="")
axis(2, at=seq(-1, 1, length.out=5), labels=round(seq(min_pitch, max_pitch, length.out=5)), line=5.4)
mtext("Pitch (degrees)", side=2, line=6.9, at=0, cex=cex)

abline(h=0, lty="solid", col="black")
legend(x=par("usr")[2], y=0.8, bty="n", col=colors, 
       lwd=1, inset=0.02, cex=cex, legend=labels, xpd=TRUE, lty=ltypes)

dev.off()
