#!/usr/bin/env ruby
require "rubygems"
require "libXML"

include LibXML

doc = XML::Parser.file("/Users/bilalh/Downloads/animelist_1293680139_-_68221.xml").parse

doc.find("//anime[contains(my_start_date/text(),'2010') and contains(my_finish_date/text(),'2010')]").each do |an|
	
	title  =  an.find('series_title/text()')[0].content
		result = `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Select EXISTS (SELECT * FROM SeriesInfo WHERE Title = '#{title.gsub /'/, "''"}')"`
		
		if result =~ /0/ then
			printf "%-60s %s    %s\n", title, an.find('my_start_date/text()')[0].content, an.find('my_finish_date/text()')[0].content
			
		end
	
end