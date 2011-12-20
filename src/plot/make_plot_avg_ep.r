#!/usr/bin/Rscript
times<-read.table("data_plot_avg_ep", header=FALSE)
names(times) <-c("scores", "counts")
attach(times)

pdf(file='plot_avg_ep.pdf')

barplot(counts,
	names.arg=scores,
	xlab="episodes",  
	ylab="Count",
	main="Distribution  of Number Episodes",
	col=rainbow(length(scores))
)