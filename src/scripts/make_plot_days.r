#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("days", header=FALSE)
names(times) <-c("days", "watched")
attach(times)
plot(days,watched, main="Episodez watched per Day", xlab="Day", ylab ="Episodes Watched")
