#!/usr/bin/Rscript
#  Plot the real time vs Time 
times<-read.table("plot_ep_scores", header=FALSE)
names(times) <-c("episodes", "score")
attach(times)
pdf(file='plot_ep_score.pdf')
plot(episodes,score, main="Score vs Number of Episodes", xlab="Episodes", ylab ="Score")
abline(lm(score~episodes), col="blue")
