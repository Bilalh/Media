#!/usr/bin/env ruby19 -wKU
require "CGI"
(puts 'adds the ep in Safari to the history'; exit) if ARGV[0] =~ /--?h(elp)?/
 

puts title = `osascript -e 'tell application "Safari"'  -e 'set my_URL to the name of document 1' -e 'end tell'`

puts ep = Integer(title[/Episode (\d+)/,1])
puts series = title[/.*?Streamers - ([- \w~:]+) :: Ep.*/,1]
series.gsub!(/\\/, '\&\&')
series.gsub!(/'/, "''")


puts "$HOME/bin/hista '#{series}' #{ep} '10 min ago' #{ep} 27 #{ARGV.join " "}"
# puts `$HOME/bin/hista '#{series}' #{ep} '10 min ago' #{ep} 27 #{ARGV.join " "}`