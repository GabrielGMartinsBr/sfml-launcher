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
        
        col = bp.width / 4
        row = bp.height / 4
        
        # vp = Viewport.new(240, 0, col, row)
        rect = Rect.new(10, 0, col, row)
        vp = Viewport.new(rect)

        
        # puts "\n"
        # puts "---Rect--------------"
        # puts "x: " + vp.rect.x.to_s
        # puts "y: " + vp.rect.y.to_s
        # puts "width: " + vp.rect.width.to_s
        # puts "height: " + vp.rect.height.to_s
        # puts "---------------------"
        # puts "\n"

        spr = Sprite.new(vp)
        spr.bitmap = bp

    end
end

def drawRect
    size = 200
    c1 = Color.new(100, 100, 100)

    spr = Sprite.new
    spr.x = 100
    spr.y = 100

    rect = Rect.new(0, 0, size, size)
    spr.bitmap = Bitmap.new(640, 480)

    # spr.bitmap.fill_rect(0, 0, size, size, Color.new(255, 255, 255))
    # spr.bitmap.fill_rect(rect, Color.new(0, 155, 255))
    spr.bitmap.fill_rect(rect, Color.new(255, 255, 255))
end



bg = Background.new
char = Character.new

drawRect

loop do
    Graphics.update
end