$w = 640
$h = 480

class Background
    def initialize
        spr = Sprite.new
        spr.bitmap = Bitmap.new($w, $h)
        
        color = Color.new(0, 0, 0)
        
        for x in 0...$w
            for y in 0...$h
                color.set(x, y, 0);
                spr.bitmap.set_pixel(x, y, color)
            end
        end
    end
end

class Character
    def initialize
        bp = Bitmap.new("Graphics/Characters/001-Fighter01")
        spr = Sprite.new
        spr.bitmap = bp
    end
end

def drawRect
    size = 128
    c1 = Color.new(100, 100, 100)

    spr = Sprite.new
    # spr.x = 100
    # spr.y = 200

    rect = Rect.new(0, 0, size, size)
    spr.bitmap = Bitmap.new(size, size)

    for x in 0...rect.width
        for y in 0...rect.height
            spr.bitmap.set_pixel(x, y, c1)
        end
    end

    # spr.bitmap.fill_rect(0, 0, size, size, Color.new(255, 255, 255))
end



bg = Background.new
char = Character.new

drawRect

loop do
    Graphics.update
end