#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain

require 'sqlite3'

(puts "#{File.basename $0} <series>"; exit) if ARGV.length == 0

name = ARGV[1..-1].join " "
name = name[1...-1]

MediaDB = File.expand_path "~/Library/Application Support/Media/Media.db"

db = SQLite3::Database.new( MediaDB )

results = db.execute(%{Select Current,Total from OngoingSeries where Series = ? }, name)
if !results  || results == []
	puts 1
	exit
end

puts results[0][0]+1