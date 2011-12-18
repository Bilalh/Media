#!/usr/bin/env ruby -wKU

DB = File.expand_path "/Users/bilalh/Library/Application Support/Media/Media.db"

puts `sqlite3 "#{DB}" 'select total,score from  FinishedSeries fs order by fs.Total;' | sed 's/|/ /'`


