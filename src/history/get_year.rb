#!/usr/bin/env ruby19 -KU
require "rubygems"
require 'sqlite3'
require 'nokogiri'
require 'open-uri'
require 'json'
require 'net/http'

#  Convert from under_scores to CamelCase
class String
	
	def inter_camelcase
	  str = dup
	  str.gsub!(/\/(.?)/){ "::#{$1.upcase}" }  # NOT SO SURE ABOUT THIS
	  str.gsub!(/(?:_+|-+)([a-z])/){ $1.upcase }
	  str
	end

	def upper_camelcase
	  inter_camelcase.gsub(/(\A|\s)([a-z])/){ $1 + $2.upcase }
	end
	
end


def check(value)
	if !value || value == "" || value == [] || value == {} then "NULL" else value  end
end

def mal_search(id)
   url = "http://mal-api.com/anime/#{id}"
   resp = Net::HTTP.get_response(URI.parse(url))
   data = resp.body

   # we convert the returned JSON data to native Ruby
   # data structure - a hash
   result = JSON.parse(data)

   # if the hash has 'Error' as a key, we raise an error
   if result.has_key? 'Error'
      raise "web service error"
   end
   return result
end

def make_table(_keys)
	keys =  _keys.map{ |x| x.upper_camelcase }
	fields = keys.map { |e| "\"#{e}\" TEXT" }.join ",\n"
	query = <<-SQL
	CREATE TABLE "MalData" (
	#{fields}
	);
	SQL
end

db = SQLite3::Database.new( File.expand_path"~/Library/Application Support/Media/Media.db" )

ids = []
db.results_as_hash = true;
db.execute( 'Select * from AllSeries where Id Is Not NULL').each do |row|
	# ids << {"id" =>(row["Id"]), "realTitle" =>(row["Title"])};
	ids << {"id" => row["Id"],};
end

ids.each do |row|
	set_row = ->(val) { row[val] =  doc.css("anime #{val.to_s}").text }
	
	# data = mal_search(row["id"])
	data = mal_search(4472)
		
	xml = `curl -u bhterra:bhterramai\#  "http://myanimelist.net/api/anime/search.xml?q=#{data["title"]}"`
	doc = Nokogiri::HTML(xml)
	date                 = doc.css("anime start_date")[0].text
	data["year"]         = date[/(\d{4}).*/,1]
	data["startAirDate"] = doc.css("anime start_date")[0].text
	data["endAirDate"]   = doc.css("anime end_date")[0].text
	data["english"]      = data["other_titles"]["english"][0] if data["other_titles"]["english"]
	data["japanese"]     = data["other_titles"]["japanese"][0] if data["other_titles"]["japanese"]
	row.merge! data
	puts row
	# p row.keys
	# break
end


useless = ["watched_episodes", "watched_status", "score", ]
now     = ["tags", "genres","other_titles" ]
later   = [ "sequels", "manga_adaptations", "prequels", "side_stories"]

	
ids.each do |h|
	_keys  = h.keys - now - useless -  later;
	# puts make_table _keys ;exit
	
	keys   = _keys.map{ |x| x.upper_camelcase }
	values = _keys.map{ |key| h[key] }
	
	qs=(['?'] * values.length).join ',';
	query = <<-SQL
	Insert Into MalData(#{keys.join ","})
	Values( #{qs})
	SQL
	
	puts query
	p db.execute(query, values.map { |e| check e } )

	# break
end
