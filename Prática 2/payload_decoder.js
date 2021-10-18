function decodeUplink(input) {
    return {
        data: {
            bytes: String.fromCharCode.apply(null, input.bytes)
        },
        warnings: [],
        errors: []
    };
}