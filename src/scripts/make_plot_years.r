#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("years", header=FALSE)
names(times) <-c("month", "watched")
attach(times)
plot(month,watched, main="Ep watched per month", xlab="months", ylab ="Ep Watched")
abline(lm(watched~month), col="blue")
