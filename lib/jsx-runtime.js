export function jsx(type, props={}, ...children) {
	children=children.flat();
	return {type, props, children};
}

export function jsxs(...args) {
	return jsx(...args);
}

export function Fragment(props) {
	//console.log(props);

	return props.children;
}
