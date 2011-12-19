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

pdf(file='plot_avg_ep_chart.pdf')

colours = rainbow(length(scores))

pie(counts,
	labels=counts,
	main="Distribution  of Number Episodes",
	col=colours
)

legend(0.8,1.0, scores, cex=0.7, fill=colours)