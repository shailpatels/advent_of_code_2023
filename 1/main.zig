const std = @import("std");
const testing = std.testing;

fn solve() !i32 {
    const in = std.io.getStdIn();
    var buf = std.io.bufferedReader(in.reader());
    var reader = buf.reader();

    while (true) {
        var msg_buf: [4096]u8 = undefined;
        var msg = try reader.skipUntilDelimiterOrEof(&msg_buf, '\n');
        std.debug.print("{s}\n", .{msg});
    }
}

test "part 1" {
    try testing.expectEqual(@as(i32, 55447), try solve());
}
