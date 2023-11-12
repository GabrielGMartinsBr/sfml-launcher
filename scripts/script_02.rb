width = 640
height = 500

color = Color.new(255, 255, 255)

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