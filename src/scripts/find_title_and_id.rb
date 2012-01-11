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

def c(s,colour=false)
	if colour then 
		require "Rainbow"
		s.color(:blue) 
	else
		s
	end
end

def find_name_and_id(name, showSQL=false, colour=false)

	# makes out by `sqlite3 mal.db  'select Synonym, Id, Total  from AllSynonyms' > out`
	lines =IO.readlines File.expand_path '~/Library/Application Support/Media/out'
	white = Text::WhiteSimilarity.new

	name = name.downcase
	results = []
	lines.each do |e|
		e.strip!
		line = e.downcase.split '|'
		short_name = name.gsub(/\s+(ova|specials?|ona|oneshot|)/i, ' ')

		if line[0].include? name or line[0].include? short_name then
			results <<  e.split( '|' )
		elsif (sim = white.similarity(name, line[0]))  > 0.6 then
			results <<  e.split('|')
		end
	end
	
	
	
	results.each do |e|
		puts "#{ "id:%-5d total:%-3d" % [e[1] || -1,  e[2] || -1]} #{c e[0], colour}"
		puts "\tset_id #{shellescape name} #{e[1] || ""} #{e[2]|| ""}" if showSQL
	end	
end

if __FILE__ == $0 then
	(puts "#{File.basename $0} <name>"; exit) if ARGV.length ==0
	find_name_and_id ARGV[0]
end