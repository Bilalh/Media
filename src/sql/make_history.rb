#!/usr/bin/env ruby
rows = `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "select Title, StartDate, EndDate from SeriesInfo si where si.finished = 1 order by si.EndDate"`.split "\n"
puts rows
rows.each_with_index do |row, i|
	data = row.split '|'
	puts `sqlite3 "/Users/bilalh/Library/Application Support/Media/Media.db" "INSERT into Watched(id,Series,DateStarted, DateFinished) values(#{i}, '#{data[0].gsub /\'/, "\'\'" }','#{data[1]}', '#{data[2]}')"`
	
end


