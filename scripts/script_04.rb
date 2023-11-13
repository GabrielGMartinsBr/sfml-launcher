Graphics.frame_rate = 50

puts Graphics.frame_rate

loop do
    # puts Graphics.frame_count

    Input.update
    Graphics.update
end