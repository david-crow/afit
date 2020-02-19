library(fpp2)

# plotting function
plot_series <- function(var, title, xlab, ylab, xmaj, xmin, ymaj, ymin) {
    par(mar=c(5.5, 8, 3, 1))
    plot(var, xlab="", ylab="", axes=FALSE)
    rect(par("usr")[1], par("usr")[3], par("usr")[2], par("usr")[4], col="gray90", border=NA)
    
    abline(v=xmaj, col="white", lwd=2)
    abline(v=xmin, col="white", lwd=1)
    abline(h=ymaj, col="white", lwd=2)
    abline(h=ymin, col="white", lwd=1)

    axis(1, at=xmaj, lwd=0, lwd.ticks=2, cex.axis=1.5)
    axis(2, at=ymaj, lwd=0, lwd.ticks=2, cex.axis=1.5, las=2)

    title(paste("(", title, ")", sep=""), adj=0, line=0.5, cex.main=2)
    title(xlab=xlab, line=3, cex.lab=2)
    title(ylab=ylab, line=6, cex.lab=2)
    lines(var, lwd=2)

    return(recordPlot())
}

# ------------------------------
# 3 x 3
# ------------------------------

# arrange the plots
par(mfrow=c(3,3))

# plot the plots
a <- plot_series(goog200, "a", "Day", "goog200",
                 seq(0,200,50), seq(25,175,50),
                 seq(400,500,50), seq(425,525,50))
b <- plot_series(diff(goog200), "b", "Day", "diff(goog200)",
                 seq(0,200,50), seq(25,175,50),
                 seq(0,60,20), seq(10,50,20))
c <- plot_series(strikes, "c", "Year", "strikes",
                 seq(1950,1980,5), seq(1952.5,1977.5,5),
                 seq(4000,6000,1000), seq(3500,5500,1000))
d <- plot_series(hsales, "d", "Year", "hsales",
                 seq(1975,1995,5), seq(1972.5,1992.5,5),
                 seq(40,80,20), seq(30,90,20))
e <- plot_series(eggs, "e", "Year", "eggs",
                 seq(1900,1980,20), seq(1910,1990,20),
                 seq(100,300,100), seq(50,350,100))
f <- plot_series(pigs, "f", "Year", "pigs",
                 seq(1980,1995,5), seq(1982.5,1992.5,5),
                 seq(50000,100000,25000), seq(37500,112500,25000))
g <- plot_series(lynx, "g", "Year", "lynx",
                 seq(1820,1920,20), seq(1830,1930,20),
                 seq(0,6000,2000), seq(1000,7000,2000))
h <- plot_series(beer, "h", "Year", "beer",
                 seq(1991,1995,1), seq(1991.5,1995.5,1),
                 seq(120,180,20), seq(130,190,20))
i <- plot_series(elec, "i", "Year", "elec",
                 seq(1960,1990,10), seq(1955,1995,10),
                 seq(4000,16000,4000), seq(2000,14000,4000))

# ------------------------------
# 2 x 3
# ------------------------------

# arrange the plots
par(mfrow=c(2,3))

# plot the plots
a <- plot_series(goog200, "a", "Day", "goog200",
                 seq(0,200,50), seq(25,175,50),
                 seq(400,500,50), seq(425,525,50))
b <- plot_series(strikes, "b", "Year", "strikes",
                 seq(1950,1980,5), seq(1952.5,1977.5,5),
                 seq(4000,6000,1000), seq(3500,5500,1000))
c <- plot_series(eggs, "c", "Year", "eggs",
                 seq(1900,1980,20), seq(1910,1990,20),
                 seq(100,300,100), seq(50,350,100))
d <- plot_series(pigs, "d", "Year", "pigs",
                 seq(1980,1995,5), seq(1982.5,1992.5,5),
                 seq(50000,100000,25000), seq(37500,112500,25000))
e <- plot_series(lynx, "e", "Year", "lynx",
                 seq(1820,1920,20), seq(1830,1930,20),
                 seq(0,6000,2000), seq(1000,7000,2000))
f <- plot_series(elec, "f", "Year", "elec",
                 seq(1960,1990,10), seq(1955,1995,10),
                 seq(4000,16000,4000), seq(2000,14000,4000))
