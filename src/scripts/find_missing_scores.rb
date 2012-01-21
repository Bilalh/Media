#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "sqlite3"

def shellescape(str)
    # An empty argument will be skipped, so return empty quotes.
    return "''" if str.empty?

    str = str.dup

    str.gsub!(/'/, %{'"'"'} )

    return %{'#{str}'}
end

DB = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

Query = <<-SQL
Select Title, Current, Total, EndDate from SeriesInfo 
Where Score isNULL and Finished = 1
SQL

rows =  DB.execute Query


rows.each do |row|
	t = Time.parse(row[3]).strftime '%a %d %b %Y'
	puts "\n*** #{row[0]}\t#{row[1]}/#{row[2]}\t#{t} ***\n"
	puts "\thists #{shellescape row[0]} "
end