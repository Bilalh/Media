#!/usr/bin/Rscript
times<-read.table("data_plot_days", header=FALSE)
names(times) <-c("days", "watched")
attach(times)

pdf(file='plot_days.pdf')

plot(days,watched, main="Episodes Watched on each Day of the Month", xlab="Day", ylab ="Episodes Watched")
abline(lm(watched~days), col="blue")