#!/usr/bin/env ruby -wKU
# Set rewatched series that are finished to watched

require "rubygems"
require 'sqlite3'

MediaDB = File.expand_path("~/Library/Application Support/Media/Media.db")

insert = <<-SQL
INSERT INTO Watched ([Series],[DateStarted], [DateFinished] )
	SELECT [Series], [StartDate], [Date]
	FROM   Rewatching r
	WHERE  r.Current = r.total
SQL

update = <<-SQL
UPDATE SeriesInfo 
SET 
		Rewatching        = 0,
		RewatchingCurrent = 0,
		RewatchingStart   = NULL,
		RewatchingDate    = NULL
WHERE Title in ( 
  	SELECT Series 
	FROM   Rewatching r
	WHERE  r.Current = r.total
)
SQL

db = SQLite3::Database.new( MediaDB )

ires = db.execute( insert )
puts ires

ures = db.execute( update )
puts ures