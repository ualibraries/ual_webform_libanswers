<?php

const LIBANSWERS_VAR_PREFIX = "libanswers_ticket_nid";

function libanswers_ticket_form($form, $form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form['#attached']['library'][] = array('webform', 'admin');
  $form['#attached']['js'][] = array('data' => array('webform' => array('revertConfirm' => t('Are you sure you want to revert any changes to your template back to the default?'))), 'type' => 'setting');

  $form['#tree'] = TRUE;
  $form['#node'] = $node;

  $form['generate_libanswers_ticket'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a ticket in Libanswers'),
    '#default_value' => variable_get(LIBANSWERS_VAR_PREFIX . $node->nid, 0)
  );
  $form['libanswers_ticket_quid'] = array(
    '#title' => t('Libanswers question id'),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => NULL,
    '#maxlength' => 500,
  );
  $form['pquestion_option'] = array(
    '#title' => t('Libanswers ticket question title'),
    '#description' => t('The question text of the ticket.'),
    '#type' => 'radios',
    '#options' => array(
      'default' =>  t('Default: %value', array('%value' => webform_replace_tokens(webform_variable_get('webform_default_subject'), $node))),
      'custom' => t('Custom'),
      'component' => t('Component')
    ),
  );
  $form['pquestion_custom'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => NULL,
    '#maxlength' => 500,
  );
  $component_options = webform_component_list($node, NULL, FALSE);
  $form['pquestion_component'] = array(
    '#type' => 'select',
    '#default_value' => NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );

  $form['pquestion_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pquestion_option']['custom']['#title'] = t('Custom: !email', array('!email' => drupal_render($form['pquestion_custom'])));
  $form['pquestion_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pquestion_option']['component']['#title'] = t('Component: !component', array('!component' => drupal_render($form['pquestion_component'])));

  $form['pname_option'] = array(
    '#title' => t('Patron name'),
    '#description' => t('Patron name associated with ticket.'),
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
    '#default_value' => NULL,
    '#maxlength' => 500,
  );
  $component_options = webform_component_list($node, NULL, FALSE);
  $form['pname_component'] = array(
    '#type' => 'select',
    '#default_value' => NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );
  $form['pname_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pname_option']['custom']['#title'] = t('Custom: !email', array('!email' => drupal_render($form['pname_custom'])));
  $form['pname_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pname_option']['component']['#title'] = t('Component: !component', array('!component' => drupal_render($form['pname_component'])));

  $form['pemail_option'] = array(
    '#title' => t('Patron e-mail address'),
    '#description' => t('Email address associated with ticket. Used for sending ticket reply information.'),
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
    '#default_value' => NULL,
    '#maxlength' => 500,
  );
  $form['pemail_custom']['#attributes']['placeholder'] = t('email@example.com');
  $form['pemail_custom']['#attributes']['class'][] = 'webform-set-active';

  $component_options = webform_component_list($node, 'email_address', FALSE);
  $form['pemail_component'] = array(
    '#type' => 'select',
    '#default_value' => NULL,
    '#options' => empty($component_options) ? array('' => t('No available components')) : $component_options,
    '#disabled' => empty($component_options),
    '#weight' => 6,
  );

  $form['pemail_option']['custom']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pemail_option']['custom']['#title'] = t('Custom: !email', array('!email' => drupal_render($form['pemail_custom'])));
  $form['pemail_option']['component']['#theme_wrappers'] = array('webform_inline_radio');
  $form['pemail_option']['component']['#title'] = t('Component: !component', array('!component' => drupal_render($form['pemail_component'])));

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
    '#default_value' => 'default',
    //'#default_value' => $email['template'] == 'default' ? 'default' : 'custom',
  );

  //$default_template = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), array('node' => $node, 'email' => $email));
  $default_template = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), array('node' => $node, 'email' => NULL));
  //$template = $email['template'] == 'default' ? $default_template : $email['template'];
  $template = $default_template;
  $form['libanswers_ticket_template_fields']['template'] = array(
    '#type' => 'textarea',
    '#rows' => max(10, min(20, count(explode("\n", $template)))),
    '#default_value' => $template,
    '#wysiwyg' => webform_variable_get('webform_email_html_capable') ? NULL : FALSE,
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

  //$form['#validate'] = array('webform_email_address_validate', 'webform_email_edit_form_validate');
  //drupal_render_children($form);
return $form;
}
