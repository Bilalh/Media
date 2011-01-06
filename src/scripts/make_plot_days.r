#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("days", header=FALSE)
names(times) <-c("days", "watched")
attach(times)
plot(days,watched, main="Ep watched per Day", xlab="day", ylab ="Ep Watched")
