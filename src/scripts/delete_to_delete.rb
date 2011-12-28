#!/usr/bin/env ruby19 -wKU

require 'sqlite3'

db    = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Mal.db" )

db.results_as_hash = false;

rows  = db.execute( 'Select Id from MalDeleted')
rows.flatten!

Dat = <<-SQL
Delete 
From MalData 
Where Id = ?
SQL

Genre = <<-SQL
Delete 
From MalGenres
Where SeriesId = ?
SQL

Tags = <<-SQL
Delete 
From MalTags
Where SeriesId = ?
SQL

Relation = <<-SQL
Delete 
From MalRelation
Where SeriesId = ?
SQL

Synonyms = <<-SQL
Delete 
From MalSynonyms
Where SeriesId = ?
SQL

rows.each do |id|
	db.execute Dat, id
	db.execute Genre, id
	db.execute Tags, id
	db.execute Relation, id
	db.execute Synonyms, id
end