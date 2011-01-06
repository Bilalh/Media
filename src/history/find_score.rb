#!/usr/bin/env ruby
require "rubygems"
require "libXML"

include LibXML

Path = "/Users/bilalh/Downloads/animelist_1294256377_-_68221.xml"

doc = XML::Parser.file(Path).parse

doc.find("//anime[my_status='Completed']").each do |an|
	
	title  =  an.find('series_title/text()')[0].content		
	
	result = `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "Select EXISTS (SELECT * FROM SeriesInfo WHERE Title = '#{title.gsub /'/, "''"}')"`
	
	
	
	if result =~ /0/ then
		
		score = an.find('my_score/text()')[0].content
		
		printf "%-80s %s\n", title, score
				
	end
	
end