width = 400
height = 300

color = Color.new(55, 100, 255)

puts color.red

color.red = 20


puts "color:"
puts color.red
puts color.green
puts color.blue

color.set(33, 33, 33)

puts "color:"
puts color.red
puts color.green
puts color.blue

# sprite = Sprite.new
# sprite.bitmap = Bitmap.new(width, height)

# for x in 0...width
#     sprite.bitmap.set_pixel(x, 64, color)
# end

# loop do
#     Graphics.update
# end