#!/usr/bin/env ruby19 -wKU
# Bilal Syed Hussain
require "text"

def find_name_and_id(name)

	white = Text::WhiteSimilarity.new

	lines =IO.readlines File.expand_path '~/Library/Application Support/Media/out'

	results = []
	lines.each do |e|
		e.strip!
		line = e.split '|'
		short_name = name.gsub(/\s+(ova|specials?|ona|oneshot|)/i, ' ')

		line[0].include? name or 
		line[0].include? short_name or
		results << line if white.similarity(name, line[0])  > 0.6
	end
	results.each { |e| puts "#{ "id:%-5d total:%-3d" % [e[1] || -1,  e[2] || -1]} #{e[0]}" }	
end

if __FILE__ == $0 then
	(puts "#{File.basename $0} <name>"; exit) if ARGV.length ==0
	find_name_and_id ARGV[0]
end