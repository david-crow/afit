library(scales)
# image_dir <- "/Users/david/Documents/masters/thesis/images/"
image_dir <- "/Users/david/Desktop/"

# ---------------------------------------------------------------------
# avas
# ---------------------------------------------------------------------

# load data
df <- read.csv("/Users/david/Documents/masters/thesis/data/avas/edm/avas-granger.csv")

# parameters
labels <- unique(df$Label)
pretty_labels <- gsub("Granger-causes", "G-causes", labels)

points <- c(0, 3, 2, 4, 5, 6)
colors <- c(hue_pal()(length(labels)),
            rgb(0/8, 0/8, 0/8, 1), rgb(1/8, 1/8, 1/8, 1), rgb(2/8, 2/8, 2/8, 1),
            rgb(3/8, 3/8, 3/8, 1), rgb(4/8, 4/8, 4/8, 1), rgb(5/8, 5/8, 5/8, 1))
margins <- c(2.2, 1.9, 0.3, 0.5)
mgp_params <- c(1.2, 0.5, 0)
cex <- 0.8

files <- c("AVASColorGrangerCausality.pdf", "AVASBWGrangerCausality.pdf")
for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=3)
    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)

    dfl <- df[df$Label == labels[1],]
    plot(dfl$P ~ dfl$Lags, ylim=c(min(df$P), 0.05), pch=points[1], col=colors[(i - 1) * 6 + 1], xaxt="n", xlab="Lag (seconds)", ylab="p")

    for (j in 2:6) {
        dfl <- df[df$Label == labels[j],]
        points(dfl$P ~ dfl$Lags, pch=points[j], col=colors[(i - 1) * 6 + j], xaxt="n")
    }

    axis(1, at=unique(df$Lags), labels=unique(df$Lags) / 10)
    abline(h=0.05, lty="dashed", col="black")
    legend("right", bty="n", inset=0.02, cex=cex, legend=pretty_labels, pch=points, col=colors[(i - 1) * 6 + 1:6])
    dev.off()
}

# ---------------------------------------------------------------------
# steering
# ---------------------------------------------------------------------

# load data
df <- read.csv("/Users/david/Documents/masters/thesis/data/steering/steering-granger.csv")

# parameters
labels <- unique(df$Label)
pretty_labels <- gsub("&", "G-causes", labels)
pretty_labels <- gsub("wheel RPM", "RPM", pretty_labels)
pretty_labels <- gsub("Steering wheel angle", "Steering", pretty_labels)

files <- c("SteeringSystemColorGrangerCausality.pdf", "SteeringSystemBWGrangerCausality.pdf")
for (i in 1:2) {
    pdf(paste(image_dir, files[i], sep=""), width=9, height=3)
    par(mar=margins, mgp=mgp_params, cex.lab=cex, cex.axis=cex, cex.main=cex)

    dfl <- df[df$Label == labels[1],]
    plot(dfl$P ~ dfl$Lags, ylim=c(min(df$P), max(df$P)), pch=points[1], col=colors[(i - 1) * 6 + 1], xaxt="n", xlab="Lag (seconds)", ylab="p")

    for (j in 2:6) {
        dfl <- df[df$Label == labels[j],]
        points(dfl$P ~ dfl$Lags, pch=points[j], col=colors[(i - 1) * 6 + j], xaxt="n")
    }

    axis(1, at=unique(df$Lags))
    abline(h=0.05, lty="dashed", col="black")
    legend("right", bty="n", inset=0.02, cex=cex, legend=pretty_labels, pch=points, col=colors[(i - 1) * 6 + 1:6])
    dev.off()
}

