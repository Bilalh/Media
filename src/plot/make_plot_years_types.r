#!/usr/bin/Rscript
name="plot_years_types"

data<-read.table(paste("data_", name, sep=""), header=T, sep="\t")
# names(data) <-c("scores", "counts")
# attach(data)
# 
pdf(file=paste(name, ".pdf", sep="") )

colours=terrain.colors(6)
barplot(t(data),
	xlab="Year",  
	ylab="Count",
	main="Distribution  of Types by Year",
	col=colours,
)
legend("topleft", names(data), cex=0.6, fill=colours)

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
