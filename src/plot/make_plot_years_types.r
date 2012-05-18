#!/usr/bin/Rscript
name="plot_years_types"

data<-read.table(paste("data_", name, sep=""), header=T, sep="\t")
# names(data) <-c("scores", "counts")
# attach(data)
# 
pdf(width=15, height=10,file=paste(name, ".pdf", sep="") )

colours=terrain.colors(6)
barplot(t(data),
	xlab="Year",  
	ylab="Count",
	main="Distribution  of Types by Year",
	col=colours,
	names.arg=c(1979,1980,1981,1982,1983,1984,1985,1986,1987,1988,1989,1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012),
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
