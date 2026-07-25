import {
  StyleSheet,
  Text,
  TouchableOpacity,
  type TouchableOpacityProps,
} from 'react-native';

type ControlButtonProps = TouchableOpacityProps;

function ControlButton({ style, children, ...props }: ControlButtonProps) {
  return (
    <TouchableOpacity style={[styles.controlButton, style]} {...props}>
      <Text style={styles.buttonText} children={children} />
    </TouchableOpacity>
  );
}

const styles = StyleSheet.create({
  controlButton: {
    width: 60,
    height: 60,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#009bff',
  },
  buttonText: {
    color: '#fff',
  },
});

export { ControlButton };
