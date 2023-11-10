width = 640
height = 500

color = Color.new(255, 255, 255)

# sprite = Sprite.new

# sprite.bitmap = Bitmap.new(width, height)

# sprite.bitmap.set_pixel(100, 64, color)
# puts sprite.bitmap.get_pixel(100, 64).blue

# for x in 0...width
#     for y in 0...64
#         sprite.bitmap.set_pixel(x, 128 + y, color)
#         sprite.bitmap.set_pixel(x, 128 + y, color)
#         sprite.bitmap.set_pixel(x, 128 + y, color)
#     end
# end

# puts sprite.bitmap.get_pixel(90, 50).blue


my_sprite = Sprite.new()
my_sprite.bitmap = Bitmap.new(width+1, height)
my_color = Color.new(0, 0, 0)

for x in 0..width
    for y in 0...height
        my_color.set(0, x, y)
        my_sprite.bitmap.set_pixel(x, y, my_color)
    end
end



loop do
    Graphics.update
end