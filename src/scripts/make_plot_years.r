#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("years", header=FALSE)
names(times) <-c("month", "watched")
attach(times)
plot(month,watched, main="Episodes watched per month", xlab="Months", ylab ="Episodes Watched")
abline(lm(watched~month), col="blue")
