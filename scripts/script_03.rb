width = 640
height = 500

color1 = Color.new(10, 100, 200)
color2 = Color.new(10, 255, 100)

sprite = Sprite.new
sprite.bitmap = Bitmap.new(width, height)

sprite2 = Sprite.new
sprite2.bitmap = Bitmap.new(width, height)

# sprite.bitmap.set_pixel(100, 64, color1)

for x in 0...width
    sprite.bitmap.set_pixel(x, 30, color1)
    sprite2.bitmap.set_pixel(x, 50, color2)
end

acc = 0
c = 0

loop do
    c+=1
    if acc < 500
        acc += 1
    end

    if c > 3000000
        sprite2.bitmap.dispose()
    end
    sprite.bitmap.set_pixel(acc, 256, color1)

    sprite2.bitmap.set_pixel(300, 300, color2)

    Graphics.update
end