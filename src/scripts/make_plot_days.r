#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("plot_days", header=FALSE)
names(times) <-c("days", "watched")
attach(times)
pdf(file='plot_days.pdf')
plot(days,watched, main="Episodes watched per Day", xlab="Day", ylab ="Episodes Watched")

