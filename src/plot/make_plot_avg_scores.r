#!/usr/bin/Rscript
times<-read.table("data_plot_avg_scores", header=FALSE)
names(times) <-c("scores", "counts")
attach(times)

colours = rainbow(length(scores))
pdf(file='plot_avg_scores.pdf')

barplot(counts,
	names.arg=scores,
	xlab="Score",  
	ylab="Count",
	main="Distribution  of Scores",
	col=rainbow(length(scores))
)

legend(0.5,80.0, counts, cex=0.6, fill=colours)


pdf(file='plot_avg_scores_chart.pdf')


pie(counts,
	labels=counts,
	main="Distribution  of Scores",
	col=colours
)

legend(0.8,1.0, scores, cex=0.7, fill=colours)