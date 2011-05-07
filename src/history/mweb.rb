#!/usr/bin/env ruby -wKU
require "CGI"
(puts 'goes to the next ep in Safari to the history'; exit) if ARGV[0] =~ /--?h(elp)?/
 

title = `osascript -e 'tell application "Safari"'  -e 'set my_URL to the name of document 1' -e 'end tell'`
ep = Integer(title[/Episode (\d+)/,1])

series = title[/.*?Streamers - ([- \w~:]+) :: Ep.*/,1]
series.gsub!(/\\/, '\&\&')
series.gsub!(/'/, "''")

url = `osascript -e 'tell application "Safari"'  -e 'set my_URL to the URL in document 1' -e 'end tell'`
puts url = CGI::unescape(url)

puts new_url = url.gsub(/ep\d+-/,"ep#{ep+1}-")
`osascript -e 'tell application "Safari"' -e 'set URL in document 1 to "#{new_url}"' -e 'end tell'`

puts "$HOME/bin/hista '#{series}' #{ep+1} '1 min ago' #{ep+1} 27 #{ARGV.join " "}"
puts `$HOME/bin/hista '#{series}' #{ep+1} '1 min ago' #{ep+1} 27 #{ARGV.join " "}`