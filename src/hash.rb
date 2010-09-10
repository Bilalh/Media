#!/usr/bin/env ruby1.9
require "facets"

narr = [];
File.open("zzhasha").each do |e|
	temp = e.split("\t")
	str = sprintf  "%d %2d %s\t%s", temp[0].length,temp[1].length-1, temp[0], temp[1]
	narr << str
end


File.writelines("zzhashb",narr );