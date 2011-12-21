#!/usr/bin/Rscript
name="plot_years"

times<-read.table(paste("data_", name, sep=""), header=FALSE)
names(times) <-c("scores", "counts")
attach(times)

pdf(width=15, height=10,file=paste(name, ".pdf", sep="") )

barplot(counts,
	names.arg=scores,
	xlab="Year",  
	ylab="Count",
	main="Distribution  of Years",
	col=rainbow(length(scores))
)

# pdf(file=paste(name, "_chart.pdf", sep=""))
# 
# colours = rainbow(length(scores))
# 
# pie(counts,
# 	labels=counts,
# 	main="Distribution of Years",
# 	col=colours
# )
# 
# legend(0.8,1.0, scores, cex=0.7, fill=colours)