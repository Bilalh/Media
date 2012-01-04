#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "text"

def shellescape(str)
    # An empty argument will be skipped, so return empty quotes.
    return "''" if str.empty?

    str = str.dup

    str.gsub!(/'/, %{'"'"'} )

    return %{'#{str}'}
end

def find_name_and_id(name, showSQL=false)

	# makes out by `sqlite3 mal.db  'select Synonym, Id, Total  from AllSynonyms' > out`
	lines =IO.readlines File.expand_path '~/Library/Application Support/Media/out'
	white = Text::WhiteSimilarity.new

	results = []
	lines.each do |e|
		e.strip!
		line = e.split '|'
		short_name = name.gsub(/\s+(ova|specials?|ona|oneshot|)/i, ' ')

		line[0].include? name or 
		line[0].include? short_name or
		results << line if white.similarity(name, line[0])  > 0.6
	end
	results.each do |e|
		puts "#{ "id:%-5d total:%-3d" % [e[1] || -1,  e[2] || -1]} #{e[0]}"
		puts "\tset_id #{shellescape name} #{e[1] || ""} #{e[2]|| ""}" if showSQL
	end	
end

if __FILE__ == $0 then
	(puts "#{File.basename $0} <name>"; exit) if ARGV.length ==0
	find_name_and_id ARGV[0]
end