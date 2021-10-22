function decodeUplink(input) {
    return {
        data: {
            counter: (input.bytes[0] << 8) + input.bytes[1]
        },
        warnings: [],
        errors: []
    };
}