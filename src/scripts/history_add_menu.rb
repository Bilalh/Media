#!/usr/bin/env ruby -wKU
require "rubygems"
require 'highline/import'

begin
titles = `$HOME/scripts/get_titles.sh`.split("\n").sort
titles.each_with_index do |e, i|
	e.strip!
	say("<%= color('#{'%-2s' % i }', BLUE) %> #{'%-2s' %  `$HOME/scripts/get_number.sh \"#{e}\"`.strip} #{e}")
end

num   = ask("Number? ", Integer) { |q| q.in = 0..titles.length }
title = titles[num]

default = Integer(`$HOME/scripts/get_number.sh '#{title}'`)+1
args= 
	ask("[lower_num] [time] [upper_num] [sep|t] [score|t] [t]") do |q|
		q.default = default
		q.validate = lambda do |ans|
			return (
				ans == default or 
				ans =~ /\d+|(\d+ [\w ]+)|(\d+ [\w ]+ [\dt]+( [\dt]+)?( t)?)/
			)
		end
	end

unless args.to_s == default.to_s
	args[/^\d+ ([\w ]+?)( [\dt]+)?( [\dt]+)?( t)?$/,1] = 
		"'" + args[/^\d+ ([\w ]+?)( [\dt]+)?( [\dt]+)?( t)?$/,1] + "'" 
end

puts "hista \"#{title}\" #{args}"
puts `$HOME/bin/hista "#{title}" #{args}`



rescue Interrupt, EOFError
end