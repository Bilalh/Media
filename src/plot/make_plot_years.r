#!/usr/bin/Rscript
times<-read.table("data_plot_years", header=FALSE)
names(times) <-c("month", "watched")
attach(times)

pdf(file='plot_years.pdf')

plot(month,watched, main="Episodes Watched on each Month of the Year", xlab="Months", ylab ="Episodes Watched")
abline(lm(watched~month), col="blue")
