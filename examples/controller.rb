lib = File.expand_path(File.dirname(__FILE__) + '/../lib')
$LOAD_PATH.unshift(lib) if File.directory?(lib) && !$LOAD_PATH.include?(lib)
require 'leap_motion'

controller = LeapMotion::Controller.new
controller.each_frame do |frame|
  puts "active hands #{frame.hands.count}"
  puts "active gestures #{frame.gestures.count}"
  frame.gestures.each do |g|
    puts g.type
  end
end
