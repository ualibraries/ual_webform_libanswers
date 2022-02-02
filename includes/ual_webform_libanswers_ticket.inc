<?php

const LIBANSWERS_VAR_PREFIX = "libanswers_ticket_nid";

function libanswers_ticket_form($form, $form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form['#attached']['library'][] = array('webform', 'admin');
  $form['#attached']['js'][] = array('data' => array('webform' => array('revertConfirm' => t('Are you sure you want to revert any changes to your template back to the default?'))), 'type' => 'setting');

  $form['#tree'] = TRUE;
  $form['#node'] = $node;

  $libanswers = $node->libanswer_ticket_settings;

  $form['generate_libanswers_ticket'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a ticket in Libanswers'),
    '#default_value' => $libanswers['generate_ticket']
  );
  $form['libanswers_ticket_quid'] = array(
    '#title' => t('Libanswers question id'),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => $libanswers['generate_ticket'],
    '#maxlength' => 500,
  );
  $form['pquestion_option'] = array(
    '#title' => t('Libanswers ticket question title'),
    '#description' => t('The question text of the ticket.'),
    '#type' => 'radios',
    '#default_value' => is_numeric($libanswers['pquestion']) ? 'component' : (($libanswers['pquestion'] && $libanswers['pquestion'] != 'default') ? 'custom' : 'default'),
    '#options' => array(
      'default' =>  t('Default: %value', array('%value' => webform_replace_tokens(webform_variable_get('webform_default_subject'), $node))),
      'custom' => t('Custom'),
      'component' => t('Component')
    ),
  );
  $form['pquestion_custom'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => (!is_numeric($libanswers['pquestion']) && $libanswers['pquestion'] != 'default') ? $libanswers['pquestion'] : NULL,
    '#maxlength' => 500,
  );
  $component_options = webform_component_list($node, NULL, FALSE);
  $form['pquestion_component'] = array(
    '#type' => 'select',
    '#default_value' => is_numeric($libanswers['pquestion']) ? $libanswers['pquestion'] : NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );

  $form['pname_option'] = array(
    '#title' => t('Patron name'),
    '#description' => t('Patron name associated with ticket.'),
    '#default_value' => is_numeric($libanswers["pname"]) ? 'component' : (($libanswers["pname"] && $libanswers["pname"] != 'default') ? 'custom' : 'default'),
    '#type' => 'radios',
    '#options' => array(
      'default' =>  t('Default: University of Arizona Libraries'),
      'custom' => t('Custom'),
      'component' => t('Component')
    ),
  );
  $form['pname_custom'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => (!is_numeric($libanswers['pname']) && $libanswers['pname'] != 'default') ? $libanswers['pname'] : NULL,
    '#maxlength' => 500,
  );
  $component_options = webform_component_list($node, NULL, FALSE);
  $form['pname_component'] = array(
    '#type' => 'select',
    '#default_value' => is_numeric($libanswers['pname']) ? $libanswers['pname'] : NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );

  $form['pemail_option'] = array(
    '#title' => t('Patron e-mail address'),
    '#description' => t('Email address associated with ticket. Used for sending ticket reply information.'),
    '#default_value' => is_numeric($libanswers['pemail']) ? 'component' : (($libanswers['pemail'] && $libanswers['pemail'] != 'default') ? 'custom' : 'default'),
    '#type' => 'radios',
    '#options' => array(
      'default' =>  t('Default: lbry-webadmin@email.arizona.edu'),
      'custom' => t('Custom'),
      'component' => t('Component')
    ),
  );
  $form['pemail_custom'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => (!is_numeric($libanswers['pemail']) && $libanswers['pemail'] != 'default') ? $libanswers['pemail'] : NULL,
    '#maxlength' => 500,
  );
  $form['pemail_custom']['#attributes']['placeholder'] = t('email@example.com');
  $form['pemail_custom']['#attributes']['class'][] = 'webform-set-active';

  $component_options = webform_component_list($node, 'email_address', FALSE);
  $form['pemail_component'] = array(
    '#type' => 'select',
    '#default_value' => is_numeric($libanswers['pemail']) ? $libanswers['pemail'] : NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );

  $form['libanswers_ticket_template_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('LibAnswers ticket template'),
    '#collapsible' => TRUE,
//      '#collapsed' => !empty($email['cid']) && empty($email['template']),
    '#description' => t('An e-mail template can customize the display of e-mails.'),
    '#weight' => 15,
    '#tree' => FALSE,
    '#attributes' => array('id' => 'webform-template-fieldset'),
  );
  $form['libanswers_ticket_template_fields']['template_option'] = array(
    '#type' => 'select',
    '#options' => array(
      'default' => t('Default template'),
      'custom' => t('Custom template'),
    ),
    '#default_value' => $libanswers['template'] == 'default' ? 'default' : 'custom'
  );

  $default_template = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), array('node' => $node, 'email' => NULL));
  $template = $default_template;
  $form['libanswers_ticket_template_fields']['template'] = array(
    '#type' => 'textarea',
    '#rows' => max(10, min(20, count(explode("\n", $template)))),
    '#default_value' => $libanswers['template'] == 'default' ? $default_template : $libanswers['template'],
    '#wysiwyg' => FALSE,
    '#description' => theme('webform_token_help', array('groups' => array('node', 'submission'))),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 20,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Libanswers ticket settings'),
  );

  $form['#validate'] = array('libanswers_ticket_email_edit_form_validate');

  return $form;
}
/**
 * Theme the Webform mail settings section of the node form.
 */
function theme_libanswers_ticket_form($variables)
{
  $form = $variables['form'];

  $form['pname_custom']['#attributes']['class'][] = 'webform-set-active';
  $form['pname_custom']['#theme_wrappers'] = array();
  $form['pname_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pname_option']['custom']['#title'] = t('!title: !field', array('!title' => $form['pname_option']['custom']['#title'], '!field' => drupal_render($form['pname_custom'])));

  $form['pname_component']['#attributes']['class'][] = 'webform-set-active';
  $form['pname_component']['#theme_wrappers'] = array();
  $form['pname_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pname_option']['component']['#title'] = t('!title: !field', array('!title' => $form['pname_option']['component']['#title'], '!field' => drupal_render($form['pname_component'])));

  $form['pemail_custom']['#attributes']['class'][] = 'webform-set-active';
  $form['pemail_custom']['#theme_wrappers'] = array();
  $form['pemail_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pemail_option']['custom']['#title'] = t('!title: !field', array('!title' => $form['pemail_option']['custom']['#title'], '!field' => drupal_render($form['pemail_custom'])));

  $form['pemail_component']['#attributes']['class'][] = 'webform-set-active';
  $form['pemail_component']['#theme_wrappers'] = array();
  $form['pemail_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pemail_option']['component']['#title'] = t('!title: !field', array('!title' => $form['pemail_option']['component']['#title'], '!field' => drupal_render($form['pemail_component'])));

  $form['pquestion_custom']['#attributes']['class'][] = 'webform-set-active';
  $form['pquestion_custom']['#theme_wrappers'] = array();
  $form['pquestion_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pquestion_option']['custom']['#title'] = t('!title: !field', array('!title' => $form['pquestion_option']['custom']['#title'], '!field' => drupal_render($form['pquestion_custom'])));

  $form['pquestion_component']['#attributes']['class'][] = 'webform-set-active';
  $form['pquestion_component']['#theme_wrappers'] = array();
  $form['pquestion_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pquestion_option']['component']['#title'] = t('!title: !field', array('!title' => $form['pquestion_option']['component']['#title'], '!field' => drupal_render($form['pquestion_component'])));

  // Ensure templates are completely hidden.
  $form['templates']['#prefix'] = '<div id="webform-email-templates" style="display: none">';
  $form['templates']['#suffix'] = '</div>';

  // Re-sort the elements since we added the details fieldset.
  $form['#sorted'] = FALSE;

  $children = element_children($form, TRUE);
  return drupal_render_children($form, $children);
}
/**
 * Validate handler for libanswers_ticket_form().
 */
function libanswers_ticket_email_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['pemail_option'] == 'custom') {
    webform_email_validate($form_state['values']['pemail_custom'], 'pemail_custom', FALSE, TRUE, TRUE);
  }
}

function set_libanswers_ticket_settings($form_values, $nid) {
  $quid = $form_values['libanswers_ticket_quid'];

  $pemail_option = $form_values['pemail_option'];

  if($pemail_option == 'component') {
    $pemail = $form_values['pemail_component'];
  } elseif ($pemail_option == 'custom') {
    $pemail = $form_values['pemail_custom'];
  } else {
    $pemail = 'default';
  }

  $pname_option = $form_values['pname_option'];
  if($pname_option == 'component') {
    $pname = $form_values['pname_component'];
  } elseif ($pname_option == 'custom') {
    $pname = $form_values['pname_custom'];
  } else {
    $pname = 'default';
  }

  $pquestion_option = $form_values['pquestion_option'];
  if($pquestion_option == 'component') {
    $pquestion = $form_values['pquestion_component'];
  } elseif ($pquestion_option == 'custom') {
    $pquestion = $form_values['pquestion_custom'];
  } else {
    $pquestion = 'default';
  }

  $form_values['libanswers_ticket_template_fields']['template'] = str_replace(array("\r", "\n"), array('', "\n"), $form_values['template']);
  $form_values['templates']['default'] = str_replace(array("\r", "\n"), array('', "\n"), $form_values['templates']['default']);
  if(strcmp(trim($form_values['templates']['default']), trim($form_values['libanswers_ticket_template_fields']['template'])) == 0) {
    $template = 'default';
  } else {
    $template = $form_values['libanswers_ticket_template_fields']['template'];
  }

  $libanswer_ticket_settings = array(
    'nid' => $nid,
    'quid' => $quid,
    'pemail' => $pemail,
    'pname' => $pname,
    'pquestion' => $pquestion,
    'template' => $template,
  );
  return $libanswer_ticket_settings;
}
/**
 * Submit handler for libanswers_ticket_form().
 */
function libanswers_ticket_form_submit($form, &$form_state) {
  // Ensure a webform record exists.
  $node = $form['#node'];
  webform_ensure_record($node);

  $libanswer_ticket_settings = set_libanswers_ticket_settings($form_state['values'], $node->nid);

  if($node->libanswer_ticket_settings == null) {
    drupal_write_record('ual_webform_libanswers_tickets', $libanswer_ticket_settings);
  } else {
    drupal_write_record('ual_webform_libanswers_tickets', $libanswer_ticket_settings, array('nid'));
  }

  entity_get_controller('node')->resetCache(array($node->nid));
  $form_state['redirect'] = array('node/' . $node->nid . '/webform/libanswers');
}
