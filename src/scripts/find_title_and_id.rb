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

def cn(float,colour=false)
	
	s = "%3.0f%%" % (p=float*100)
	
	if colour then 
		require "Rainbow"
		case p
		when 0...70
			s.bright
		when 70...80
			s.color(:red).bright
		when 80...90
			s.color(:green).bright
		when 90..100
			s.color(:yellow).underline.bright
		end
		
	else
		s
	end
	
end

def find_name_and_id(name, showSQL=false, colour=false)

	# makes out by `sqlite3 mal.db  'select Synonym, Id, Total  from AllSynonyms' > out`
	lines =IO.readlines File.expand_path '~/Library/Application Support/Media/out'
	white = Text::WhiteSimilarity.new

	title = name.downcase
	results = []
	lines.each do |e|
		e.strip!
		line = e.downcase.split '|'
		short_name = title.gsub(/\s+(ova|specials?|ona|oneshot|)/i, ' ')

		if line[0].include? title or line[0].include? short_name then
			results <<  [1] + e.split( '|' )
		elsif (sim = white.similarity(title, line[0]))  > 0.6 then
			results <<  [sim]  + e.split('|')
		end
	end
	
	
	
	results.each do |e|
		puts "#{ "%s id:%-5d total:%-3d" % [cn(e[0], colour), e[2] || -1,  e[3] || -1]} #{c e[1], colour}"
		puts "\tset_id #{shellescape name} #{e[2] || ""} #{e[3]|| ""}" if showSQL
	end	
end

if __FILE__ == $0 then
	(puts "#{File.basename $0} <name>"; exit) if ARGV.length ==0
	find_name_and_id ARGV[0]
end