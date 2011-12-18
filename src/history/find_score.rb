#!/usr/bin/env ruby -KU
require "rubygems"
require "libXML"
require 'sqlite3'

include LibXML

Path = File.expand_path "~/Downloads/animelist.xml"
db = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

doc = XML::Parser.file(Path).parse


doc.find("//anime").each do |an|
	
	title      = an.find('series_title/text()')[0].content
	score      = an.find('my_score/text()')[0].content
	id         = an.find('series_animedb_id/text()')[0].content
	start_date = an.find('my_start_date/text()')[0].content
	end_date   = an.find('my_finish_date/text()')[0].content

	
	exists = db.execute( 'Select EXISTS (SELECT * FROM OldSeries WHERE Title = ?)',title ).flatten[0]
	puts "#{score} exists:#{exists}  #{title}"
	if exists == 1 then
		db.execute( 'Update OldSeries Set Score = ?, Id = ? where title = ?', [score, id, title] )
		puts "\tScore #{score} of #{title} set"
	end
	
	if start_date != "0000-00-00" then
		db.execute( 'Update OldSeries Set StartDate = ? where title = ?', start_date+ " 00:00:00", title)
	end 
	if end_date != "0000-00-00" then
		db.execute( 'Update OldSeries Set EndDate = ? where title = ?', end_date + " 00:00:00", title)
	end
	
end