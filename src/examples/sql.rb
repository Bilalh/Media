#!/usr/bin/env ruby
require "rubygems"
require 'sqlite3'
MEDIA_DB ="/Users/bilalh/Library/Application Support/Media/Media.db"

db = SQLite3::Database.new( MEDIA_DB )
db.results_as_hash = true;

rows = db.execute( "select * from OngoingSeries" )

rows.each do |row|
	puts row['Series']
end