#!/usr/bin/env ruby -wKU
require "rubygems"
require 'highline/import'

begin

titles = `$HOME/bin/ongoingdata`.split("\n").sort
titles.each_index do |i|
	titles[i].strip!
	titles[i]=titles[i].split("\t")
	say("<%= color('#{'%-2s' % i }', BLUE) %> #{'%-2s' %  titles[i][1]} #{titles[i][0]}")
end

num   = ask("Number? ", Integer) { |q| q.in = 0..titles.length }
title = titles[num][0]

default = Integer(titles[num][1])+1
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