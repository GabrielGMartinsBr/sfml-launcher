Graphics.frame_rate = 60
# puts Graphics.frame_rate


color = Color.new(10, 100, 200)

sprite = Sprite.new
sprite.bitmap = Bitmap.new(640, 480)


i = 0

loop do
    Graphics.update

    i += 1
    if i == 640
        puts Graphics.frame_count
    end

    sprite.bitmap.set_pixel(i, 300, color);

    # Input.update
    # Graphics.update
end