#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("plot_years", header=FALSE)
names(times) <-c("month", "watched")
attach(times)
pdf(file='plot_years.pdf')
plot(month,watched, main="Episodes watched per month", xlab="Months", ylab ="Episodes Watched")
abline(lm(watched~month), col="blue")
