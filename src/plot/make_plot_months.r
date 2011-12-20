#!/usr/bin/Rscript
times<-read.table("data_plot_months", header=FALSE)
names(times) <-c("month", "watched")
attach(times)

pdf(file='plot_months.pdf')

plot(month,watched, main="Episodes Watched on each Month of the Year", xlab="Months", ylab ="Episodes Watched")
abline(lm(watched~month), col="blue")
