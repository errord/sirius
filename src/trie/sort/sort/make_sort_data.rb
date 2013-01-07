#!/home/error.d/ruby/bin/ruby -w

def sample(n, m)
  s = []
  ((n-m)...n).each do |j|
    t = rand(j+2)
    s.concat s.include?(t) ? [j+1] : [t]
  end
  s
end


sort_data = sample(10000000, 1000000);
#file = File.new("data", "w")
sort_data.each{|n| print n, ","}
