#!/usr/bin/env ruby -wKU
(puts 'adds the ep in Safari to the history'; exit) if ARGV[0] =~ /--?h(elp)?/
 

url = `osascript -e 'tell application "Safari"'  -e 'set my_URL to the URL in document 1' -e 'end tell'`
series  = url[/a=(\w+)/,1].tr '_', ' '
ep = Integer(url[/ep=(\d+)/,1])

new_url = url.gsub(/ep=\d+/,"ep=#{ep+1}")
`osascript -e 'tell application "Safari"' -e 'set URL in document 1 to "#{new_url}"' -e 'end tell'`

puts `$HOME/bin/hista '#{series}' #{ep+1}, '1 min ago' #{ep} #{ARGV[0]}`