#!/usr/bin/env ruby -wKU
require "rubygems"
require 'highline/import'

begin
titles = `$HOME/bin/get_titles.sh`.split("\n").sort
titles.each_with_index do |e, i|
	e.strip!
	say("<%= color('#{'%-2s' % i }', BLUE) %> #{'%-2s' %  `$HOME/scripts/get_number.sh \"#{e}\"`.strip} #{e}")
end

num   = ask("Number? ", Integer) { |q| q.in = 0..titles.length }
title = titles[num]
`echo "#{title}" | pbcopy `

rescue Interrupt, EOFError
end