#!/usr/bin/env ruby -wKU
(puts 'adds the ep in Safari to the history'; exit) if ARGV[0] =~ /--?h(elp)?/
 

url = `osascript -e 'tell application "Safari"'  -e 'set my_URL to the URL in document 1' -e 'end tell'`
series  = url[/a=([-\w~_]+)/,1].tr '_', ' '
ep = Integer(url[/ep=(\d+)/,1])

puts "$HOME/bin/hista '#{series}' #{ep} '1 min ago' #{ep} #{ARGV.join " "}"
puts `$HOME/bin/hista '#{series}' #{ep} '1 min ago' #{ep} #{ARGV.join " "}`